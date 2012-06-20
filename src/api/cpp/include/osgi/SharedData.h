/*=============================================================================

  Copyright (c) Sascha Zelzer

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.


Modified version of qshareddata.h from Qt 4.7.3 for NativeOSGi.
Original copyright (c) Nokia Corporation. Usage covered by the
GNU Lesser General Public License version 2.1
(http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html) and the Nokia Qt
LGPL Exception version 1.1 (file LGPL_EXCEPTION.txt in Qt 4.7.3 package).

=========================================================================*/

#pragma once
#ifndef OSGI_SHAREDDATA_H_
#define OSGI_SHAREDDATA_H_

#include <osgi/AtomicInt.h>

#include <algorithm>
#include <utility>

/** \cond internal */

namespace osgi {

class SharedData
{
public:
  mutable AtomicInt f_Ref;

  inline SharedData() : f_Ref(0) { }
  inline SharedData(const SharedData&) : f_Ref(0) { }

private:
  // using the assignment operator would lead to corruption in the ref-counting
  SharedData& operator=(const SharedData&);
};

template <class T>
class SharedDataPointer
{
public:
  typedef T Type;
  typedef T* pointer;

  inline void detach() { if (d && d->f_Ref != 1) detach_helper(); }
  inline T& operator*() { detach(); return *d; }
  inline const T& operator*() const { return *d; }
  inline T* operator->() { detach(); return d; }
  inline const T* operator->() const { return d; }
  inline operator T*() { detach(); return d; }
  inline operator const T*() const { return d; }
  inline T* data() { detach(); return d; }
  inline const T* data() const { return d; }
  inline const T* constData() const { return d; }

  inline bool operator==(const SharedDataPointer<T>& other) const { return d == other.d; }
  inline bool operator!=(const SharedDataPointer<T>& other) const { return d != other.d; }

  inline SharedDataPointer() : d(0) { }
  inline ~SharedDataPointer() { if (d && !d->f_Ref.deref()) delete d; }

  explicit SharedDataPointer(T* data);
  inline SharedDataPointer(const SharedDataPointer<T>& o) : d(o.d) { if (d) d->f_Ref.ref(); }

  inline SharedDataPointer<T> & operator=(const SharedDataPointer<T>& o)
  {
    if (o.d != d)
    {
      if (o.d)
        o.d->f_Ref.ref();
      T *old = d;
      d = o.d;
      if (old && !old->f_Ref.deref())
        delete old;
    }
    return *this;
  }

  inline SharedDataPointer &operator=(T *o)
  {
    if (o != d)
    {
      if (o)
        o->f_Ref.ref();
      T *old = d;
      d = o;
      if (old && !old->f_Ref.deref())
        delete old;
    }
    return *this;
  }

  inline bool operator!() const { return !d; }

  inline void swap(SharedDataPointer& other)
  {
    using std::swap;
    swap(d, other.d);
  }

protected:
  T* clone();

private:
  void detach_helper();

  T *d;
};

template <class T>
class ExplicitlySharedDataPointer
{
public:
  typedef T Type;
  typedef T* pointer;

  inline T& operator*() const { return *d; }
  inline T* operator->() { return d; }
  inline T* operator->() const { return d; }
  inline T* data() const { return d; }
  inline const T* constData() const { return d; }

  inline void detach() { if (d && d->f_Ref != 1) detach_helper(); }

  inline void reset()
  {
    if(d && !d->f_Ref.deref())
      delete d;

    d = 0;
  }

  inline operator bool () const { return d != 0; }

  inline bool operator==(const ExplicitlySharedDataPointer<T>& other) const { return d == other.d; }
  inline bool operator!=(const ExplicitlySharedDataPointer<T>& other) const { return d != other.d; }
  inline bool operator==(const T* ptr) const { return d == ptr; }
  inline bool operator!=(const T* ptr) const { return d != ptr; }

  inline ExplicitlySharedDataPointer() { d = 0; }
  inline ~ExplicitlySharedDataPointer() { if (d && !d->f_Ref.deref()) delete d; }

  explicit   ExplicitlySharedDataPointer(T* data);
  inline   ExplicitlySharedDataPointer(const   ExplicitlySharedDataPointer<T> &o)
    : d(o.d) { if (d) d->f_Ref.ref(); }

  template<class X>
  inline   ExplicitlySharedDataPointer(const   ExplicitlySharedDataPointer<X>& o)
    : d(static_cast<T*>(o.data()))
  {
    if(d)
      d->f_Ref.ref();
  }

  inline ExplicitlySharedDataPointer<T>& operator=(const   ExplicitlySharedDataPointer<T>& o)
  {
    if (o.d != d)
    {
      if (o.d)
        o.d->f_Ref.ref();
      T *old = d;
      d = o.d;
      if (old && !old->f_Ref.deref())
        delete old;
    }
    return *this;
  }

  inline ExplicitlySharedDataPointer& operator=(T* o)
  {
    if (o != d)
    {
      if (o)
        o->f_Ref.ref();
      T *old = d;
      d = o;
      if (old && !old->f_Ref.deref())
        delete old;
    }
    return *this;
  }

  inline bool operator!() const { return !d; }

  inline void swap(  ExplicitlySharedDataPointer& other)
  {
    using std::swap;
    swap(d, other.d);
  }

protected:
  T* clone();

private:
  void detach_helper();

  T *d;
};


template <class T>
SharedDataPointer<T>::SharedDataPointer(T* adata) : d(adata)
{ if (d) d->f_Ref.ref(); }

template <class T>
T* SharedDataPointer<T>::clone()
{
  return new T(*d);
}

template <class T>
void SharedDataPointer<T>::detach_helper()
{
  T *x = clone();
  x->f_Ref.ref();
  if (!d->f_Ref.deref())
    delete d;
  d = x;
}

template <class T>
T* ExplicitlySharedDataPointer<T>::clone()
{
  return new T(*d);
}

template <class T>
void ExplicitlySharedDataPointer<T>::detach_helper()
{
  T *x = clone();
    x->f_Ref.ref();
  if (!d->f_Ref.deref())
    delete d;
  d = x;
}

template <class T>
ExplicitlySharedDataPointer<T>:: ExplicitlySharedDataPointer(T* adata)
  : d(adata)
{ if (d) d->f_Ref.ref(); }

template <class T>
void swap(SharedDataPointer<T>& p1, SharedDataPointer<T>& p2)
{ p1.swap(p2); }

template <class T>
void swap(  ExplicitlySharedDataPointer<T>& p1, ExplicitlySharedDataPointer<T>& p2)
{ p1.swap(p2); }

} // end namespace osgi

/** \endcond */

#endif // OSGI_SHAREDDATA_H_
