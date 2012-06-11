
#pragma once
#ifndef OSGI_SERVICEEXCEPTION_H_
#define OSGI_SERVICEEXCEPTION_H_

#include <osgi/Exception.h>

#include <string>

namespace osgi {

/**
 * \ingroup cpp_api
 *
 * A service exception used to indicate that a service problem occurred.
 *
 * <p>
 * A <code>ServiceException</code> object is created by the Framework or
 * to denote an exception condition in the service. An enum
 * type is used to identify the exception type for future extendability.
 *
 * <p>
 * This exception conforms to the general purpose exception chaining mechanism.
 */
class OSGI_CPP_EXPORT ServiceException : public RuntimeException
{
public:

  enum Type {

    /**
     * No exception type is unspecified.
     */
    UNSPECIFIED,

    /**
     * The service has been unregistered.
     */
    UNREGISTERED,

    /**
     * The service factory produced an invalid service object.
     */
    FACTORY_ERROR,

    /**
     * The service factory threw an exception.
     */
    FACTORY_EXCEPTION,

    /**
     * An error occurred invoking a remote service.
     */
    REMOTE,

    /**
     * The service factory resulted in a recursive call to itself for the
     * requesting plugin.
     */
    FACTORY_RECURSION
  };

  /**
   * Creates a <code>ServiceException</code> with the specified message and
   * type.
   *
   * @param msg The associated message.
   * @param type The type for this exception.
   */
  ServiceException(const std::string& msg, Type type = UNSPECIFIED);

  /**
   * Creates a <code>ServiceException</code> with the specified message,
   * type and exception cause.
   *
   * @param msg The associated message.
   * @param type The type for this exception.
   * @param cause The cause of this exception.
   */
  ServiceException(const std::string& msg, Type type, const Exception& cause);

  /**
   * Creates a <code>ServiceException</code> with the specified message and
   * exception cause.
   *
   * @param msg The associated message.
   * @param cause The cause of this exception.
   */
  ServiceException(const std::string& msg, const Exception& cause);

  ServiceException(const ServiceException& o);
  ServiceException& operator=(const ServiceException& o);

  ~ServiceException() throw();

  /**
   * @see Exception::name()
   */
  const char* name() const throw();

  /**
   * @see Exception::clone()
   */
  ServiceException* clone() const;

  /**
   * Returns the type for this exception or <code>UNSPECIFIED</code> if the
   * type was unspecified or unknown.
   *
   * @return The type of this exception.
   */
  Type getType() const;

private:

  /**
   * Type of service exception.
   */
  Type f_Type;

};

} // end namespace osgi

#endif // OSGI_SERVICEEXCEPTION_H_
