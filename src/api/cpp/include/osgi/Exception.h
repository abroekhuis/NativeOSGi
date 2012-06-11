
#pragma once
#ifndef OSGI_EXCEPTION_H_
#define OSGI_EXCEPTION_H_

#include <osgi/Export.h>

#include <exception>
#include <string>

namespace osgi {

/**
 * \ingroup cpp_api
 *
 * \brief The base class for all OSGi related exceptions.
 *
 * This exception class takes a std::string object as the message text and can
 * optionally store another Exception instance which caused this exception.
 *
 * Exception classes can be copied, saved, and rethrown.
 */
class OSGI_CPP_EXPORT Exception : public std::exception
{
public:

  /**
   * @brief Create a new Exception.
   * @param msg The exception message.
   */
  explicit Exception(const std::string& msg);

  /**
   * @brief Create a new Exception containing another exception as the cause.
   * @param msg The exception message.
   * @param cause The nested exception causing this exception.
   */
  Exception(const std::string& msg, const Exception& cause);

  /**
   * @brief Copy constructor.
   * @param o The exception to copy.
   */
  Exception(const Exception& o);

  /**
   * @brief Assignment operator.
   * @param o The exception to assign to this exception.
   * @return
   */
  Exception& operator=(const Exception& o);

  ~Exception() throw();

  /**
   * @brief Gets the nested exception which caused this exception.
   * @return The nested exception, or <code>NULL</code> if there is none.
   */
  const Exception* cause() const throw();

  /**
   * @brief Sets the cause for this exception.
   * @param cause The exception causing this exception.
   */
  void setCause(const Exception& cause);

  /**
   * @brief Returns the name for this exception.
   * @return The exception name.
   */
  virtual const char* name() const throw();

  /**
   * @brief Returns the class name for this exception.
   * @return The exception class name.
   */
  virtual const char* className() const throw();

  /**
   * @brief Returns a static string describing this exception.
   * @return The exception description.
   */
  virtual const char* what() const throw();

  /**
   * @brief Creates a copy of this exception. Use rethrow() to throw the
   * copy again.
   * @return A copy of this exception.
   */
  virtual Exception* clone() const;

  /**
   * @brief (Re)Throws this exception.
   */
  void rethrow() const;

private:

  std::string f_Msg;
  Exception* f_NestedException;
};

} // namespace osgi

/**
 * \ingroup cpp_api
 */
OSGI_CPP_EXPORT std::ostream& operator<<(std::ostream& os, const osgi::Exception& exc);

/**
 * \ingroup cpp_api
 *
 * \brief Quickly declare a Exception sub-class.
 * \param API The export macro.
 * \param CLS The class name for the Exception sub-class.
 * \param BASE The class name of the actual super class.
 */
#define OSGI_DECLARE_EXCEPTION(API, CLS, BASE)               \
  class API CLS : public BASE                                \
  {                                                          \
  public:                                                    \
    explicit CLS(const std::string& msg);                    \
    CLS(const std::string& msg, const osgi::Exception& exc); \
    CLS(const CLS& exc);                                     \
    ~CLS() throw();                                          \
    CLS& operator = (const CLS& exc);                        \
    const char* name() const throw();                        \
    CLS* clone() const;                                      \
  };

//---------------------------------------------------------------------------
/**
 * \ingroup cpp_api
 *
 * \brief Quickly implement a Exception sub-class
 * \param CLS The class name for the Exception sub-class.
 * \param BASE The class name of the actual super class.
 * \param NAME A human-readable name for this exception class.
 */
#define OSGI_IMPLEMENT_EXCEPTION(CLS, BASE, NAME)                               \
  CLS::CLS(const std::string& msg) : BASE(msg)                                  \
  { }                                                                           \
  CLS::CLS(const std::string& msg, const osgi::Exception& exc) : BASE(msg, exc) \
  { }                                                                           \
  CLS::CLS(const CLS& exc) : BASE(exc)                                          \
  { }                                                                           \
  CLS::~CLS() throw()                                                           \
  { }                                                                           \
  CLS& CLS::operator = (const CLS& exc)                                         \
  {                                                                             \
    BASE::operator = (exc);                                                     \
    return *this;                                                               \
  }                                                                             \
  const char* CLS::name() const throw()                                         \
  {                                                                             \
    return NAME;                                                                \
  }                                                                             \
  CLS* CLS::clone() const                                                       \
  {                                                                             \
    return new CLS(*this);                                                      \
  }

namespace osgi {
OSGI_DECLARE_EXCEPTION(OSGI_CPP_EXPORT, RuntimeException, Exception)
OSGI_DECLARE_EXCEPTION(OSGI_CPP_EXPORT, InvalidArgumentException, RuntimeException)
OSGI_DECLARE_EXCEPTION(OSGI_CPP_EXPORT, IllegalStateException, RuntimeException)
}

#endif // OSGI_EXCEPTION_H_
