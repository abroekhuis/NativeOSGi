
#pragma once
#ifndef USTHREADINGMODEL_H
#define USTHREADINGMODEL_H

#include <osgi/Config.h>

/** \cond internal */

#ifdef OSGI_ENABLE_THREADING_SUPPORT

  // Atomic compiler intrinsics

  #if defined(OSGI_PLATFORM_APPLE)
    // OSAtomic.h optimizations only used in 10.5 and later
    #include <AvailabilityMacros.h>
    #if MAC_OS_X_VERSION_MAX_ALLOWED >= 1050
      #include <libkern/OSAtomic.h>
      #define OSGI_ATOMIC_OPTIMIZATION_APPLE
    #endif

  #elif defined(__GLIBCPP__) || defined(__GLIBCXX__)

    #if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 2))
      # include <ext/atomicity.h>
    #else
      # include <bits/atomicity.h>
    #endif
    #define OSGI_ATOMIC_OPTIMIZATION_GNUC

  #endif

  // Mutex support

  #ifdef OSGI_PLATFORM_WINDOWS

    #define WIN32_LEAN_AND_MEAN
    #ifndef NOMINMAX
      #define NOMINMAX
    #endif
    #include <windows.h>

    #define OSGI_THREADS_MUTEX(x)           HANDLE (x);
    #define OSGI_THREADS_MUTEX_INIT(x)
    #define OSGI_THREADS_MUTEX_CTOR(x)      : x(::CreateMutex(NULL, FALSE, NULL))
    #define OSGI_THREADS_MUTEX_DELETE(x)    ::CloseHandle (x)
    #define OSGI_THREADS_MUTEX_LOCK(x)      ::WaitForSingleObject (x, INFINITE)
    #define OSGI_THREADS_MUTEX_UNLOCK(x)    ::ReleaseMutex (x)
    #define OSGI_THREADS_LONG               LONG

    #define OSGI_ATOMIC_OPTIMIZATION
    #define OSGI_ATOMIC_INCREMENT(x)        IntType n = InterlockedIncrement(x)
    #define OSGI_ATOMIC_DECREMENT(x)        IntType n = InterlockedDecrement(x)
    #define OSGI_ATOMIC_ASSIGN(l, r)        InterlockedExchange(l, r)

  #elif defined(OSGI_PLATFORM_POSIX)

    #include <pthread.h>

    #define OSGI_THREADS_MUTEX(x)           pthread_mutex_t (x);
    #define OSGI_THREADS_MUTEX_INIT(x)      ::pthread_mutex_init(&x, 0)
    #define OSGI_THREADS_MUTEX_CTOR(x)      : x()
    #define OSGI_THREADS_MUTEX_DELETE(x)    ::pthread_mutex_destroy (&x)
    #define OSGI_THREADS_MUTEX_LOCK(x)      ::pthread_mutex_lock (&x)
    #define OSGI_THREADS_MUTEX_UNLOCK(x)    ::pthread_mutex_unlock (&x)

    #define OSGI_ATOMIC_OPTIMIZATION
    #if defined(OSGI_ATOMIC_OPTIMIZATION_APPLE)
      #if defined (__LP64__) && __LP64__
        #define OSGI_THREADS_LONG           volatile int64_t
        #define OSGI_ATOMIC_INCREMENT(x)    IntType n = OSAtomicIncrement64Barrier(x)
        #define OSGI_ATOMIC_DECREMENT(x)    IntType n = OSAtomicDecrement64Barrier(x)
        #define OSGI_ATOMIC_ASSIGN(l, v)    OSAtomicCompareAndSwap64Barrier(*l, v, l)
      #else
        #define OSGI_THREADS_LONG           volatile int32_t
        #define OSGI_ATOMIC_INCREMENT(x)    IntType n = OSAtomicIncrement32Barrier(x)
        #define OSGI_ATOMIC_DECREMENT(x)    IntType n = OSAtomicDecrement32Barrier(x)
        #define OSGI_ATOMIC_ASSIGN(l, v)    OSAtomicCompareAndSwap32Barrier(*l, v, l)
      #endif
    #elif defined(OSGI_ATOMIC_OPTIMIZATION_GNUC)
      #define OSGI_THREADS_LONG             _Atomic_word
      #define OSGI_ATOMIC_INCREMENT(x)      IntType n = __sync_add_and_fetch(x, 1)
      #define OSGI_ATOMIC_DECREMENT(x)      IntType n = __sync_add_and_fetch(x, -1)
      #define OSGI_ATOMIC_ASSIGN(l, v)      __sync_val_compare_and_swap(l, *l, v)
    #else
      #define OSGI_THREADS_LONG             long
      #undef OSGI_ATOMIC_OPTIMIZATION
      #define OSGI_ATOMIC_INCREMENT(x)      f_AtomicMtx.Lock();  \
                                            IntType n = ++(*x);  \
                                            f_AtomicMtx.Unlock()
      #define OSGI_ATOMIC_DECREMENT(x)      f_AtomicMtx.Lock();  \
                                            IntType n = --(*x);  \
                                            f_AtomicMtx.Unlock()
      #define OSGI_ATOMIC_ASSIGN(l, v)      f_AtomicMtx.Lock();  \
                                            *l = v;              \
                                            f_AtomicMtx.Unlock()
    #endif

  #endif

#else

  // single threaded
  #define OSGI_THREADS_MUTEX(x)
  #define OSGI_THREADS_MUTEX_INIT(x)
  #define OSGI_THREADS_MUTEX_CTOR(x)
  #define OSGI_THREADS_MUTEX_DELETE(x)
  #define OSGI_THREADS_MUTEX_LOCK(x)
  #define OSGI_THREADS_MUTEX_UNLOCK(x)
  #define OSGI_THREADS_LONG

#endif

#ifndef OSGI_DEFAULT_MUTEX
  #define OSGI_DEFAULT_MUTEX osgi::Mutex
#endif


namespace osgi {

class Mutex
{
public:

  Mutex() OSGI_THREADS_MUTEX_CTOR(f_Mtx)
  {
    OSGI_THREADS_MUTEX_INIT(f_Mtx);
  }

  ~Mutex()
  {
    OSGI_THREADS_MUTEX_DELETE(f_Mtx);
  }

  void Lock()
  {
    OSGI_THREADS_MUTEX_LOCK(f_Mtx);
  }
  void Unlock()
  {
    OSGI_THREADS_MUTEX_UNLOCK(f_Mtx);
  }

private:

  // Copy-constructor not implemented.
  Mutex(const Mutex &);
  // Copy-assignement operator not implemented.
  Mutex & operator = (const Mutex &);

  OSGI_THREADS_MUTEX(f_Mtx)
};

template<class MutexPolicy = OSGI_DEFAULT_MUTEX>
class MutexLock
{
public:
  typedef MutexPolicy MutexType;

  MutexLock(MutexType& mtx) : f_Mtx(&mtx) { f_Mtx->Lock(); }
  ~MutexLock() { f_Mtx->Unlock(); }

private:
  MutexType* f_Mtx;

  // purposely not implemented
  MutexLock(const MutexLock&);
  MutexLock& operator=(const MutexLock&);
};

typedef MutexLock<> DefaultMutexLock;


#ifdef OSGI_ENABLE_THREADING_SUPPORT

template<class Host, class MutexPolicy = OSGI_DEFAULT_MUTEX>
class MultiThreaded
{
  mutable MutexPolicy f_Mtx;

  #if !defined(OSGI_ATOMIC_OPTIMIZATION)
  mutable MutexPolicy f_AtomicMtx;
  #endif

public:

  MultiThreaded() : f_Mtx() {}
  MultiThreaded(const MultiThreaded&) : f_Mtx() {}
  virtual ~MultiThreaded() {}

  class Lock;
  friend class Lock;

  class Lock
  {
  public:

    // Lock object
    explicit Lock(const MultiThreaded& host) : f_Host(host)
    {
      f_Host.f_Mtx.Lock();
    }

    // Lock object
    explicit Lock(const MultiThreaded* host) : f_Host(*host)
    {
      f_Host.f_Mtx.Lock();
    }

    // Unlock object
    ~Lock()
    {
      f_Host.f_Mtx.Unlock();
    }

  private:

    // private by design
    Lock();
    Lock(const Lock&);
    Lock& operator=(const Lock&);
    const MultiThreaded& f_Host;

  };

  typedef volatile Host VolatileType;
  typedef OSGI_THREADS_LONG IntType;

  IntType AtomicIncrement(volatile IntType& lval) const
  {
    OSGI_ATOMIC_INCREMENT(&lval);
    return n;
  }

  IntType AtomicDecrement(volatile IntType& lval) const
  {
    OSGI_ATOMIC_DECREMENT(&lval);
    return n;
  }

  void AtomicAssign(volatile IntType& lval, const IntType val) const
  {
    OSGI_ATOMIC_ASSIGN(&lval, val);
  }

};

#endif

template<class Host, class MutexPolicy = OSGI_DEFAULT_MUTEX>
class SingleThreaded
{
public:

  virtual ~SingleThreaded() {}

  // Dummy Lock class
  struct Lock
  {
    Lock() {}
    explicit Lock(const SingleThreaded&) {}
    explicit Lock(const SingleThreaded*) {}
  };

  typedef Host VolatileType;
  typedef int IntType;

  static IntType AtomicAdd(volatile IntType& lval, const IntType val)
  { return lval += val; }

  static IntType AtomicSubtract(volatile IntType& lval, const IntType val)
  { return lval -= val; }

  static IntType AtomicMultiply(volatile IntType& lval, const IntType val)
  { return lval *= val; }

  static IntType AtomicDivide(volatile IntType& lval, const IntType val)
  { return lval /= val; }

  static IntType AtomicIncrement(volatile IntType& lval)
  { return ++lval; }

  static IntType AtomicDecrement(volatile IntType& lval)
  { return --lval; }

  static void AtomicAssign(volatile IntType & lval, const IntType val)
  { lval = val; }

  static void AtomicAssign(IntType & lval, volatile IntType & val)
  { lval = val; }

};

} // end namespace osgi

/** \endcond */

#endif // USTHREADINGMODEL_H
