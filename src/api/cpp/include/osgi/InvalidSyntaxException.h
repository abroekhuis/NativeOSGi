
#pragma once
#ifndef OSGI_INVALIDSYNTAXEXCEPTION_H_
#define OSGI_INVALIDSYNTAXEXCEPTION_H_

#include <osgi/Exception.h>

namespace osgi {

/**
 * \ingroup cpp_api
 *
 * A Framework exception used to indicate that a filter string has an invalid
 * syntax.
 *
 * <p>
 * An {@code InvalidSyntaxException} object indicates that a filter
 * string parameter has an invalid syntax and cannot be parsed. See
 * {@link Filter} for a description of the filter string syntax.
 *
 * <p>
 * This exception conforms to the general purpose exception chaining mechanism.
 */
class OSGI_CPP_EXPORT InvalidSyntaxException : public Exception
{
public:

  /**
   * Creates an exception of type {@code InvalidSyntaxException}.
   *
   * <p>
   * This method creates an {@code InvalidSyntaxException} object with
   * the specified message and the filter string which generated the
   * exception.
   *
   * @param msg The message.
   * @param filter The invalid filter string.
   */
  InvalidSyntaxException(const std::string& msg, const std::string& filter);

  /**
   * Creates an exception of type {@code InvalidSyntaxException}.
   *
   * <p>
   * This method creates an {@code InvalidSyntaxException} object with
   * the specified message and the filter string which generated the
   * exception.
   *
   * @param msg The message.
   * @param filter The invalid filter string.
   * @param cause The cause of this exception.
   */
  InvalidSyntaxException(const std::string& msg, const std::string& filter, const Exception& cause);

  /**
   * Returns the filter string that generated the
   * {@code InvalidSyntaxException} object.
   *
   * @return The invalid filter string.
   * @see BundleContext#getServiceReferences
   * @see BundleContext#addServiceListener(ServiceListener,std::string)
   */
  std::string getFilter() const;

  InvalidSyntaxException(const InvalidSyntaxException& o);
  InvalidSyntaxException& operator=(const InvalidSyntaxException& o);

  ~InvalidSyntaxException() throw();

  /**
   * @see Exception::name()
   */
  const char* name() const throw();

  /**
   * @see Exception::clone()
   */
  InvalidSyntaxException* clone() const;

private:

  /**
   * The invalid filter string.
   */
  std::string	f_Filter;

};

} // end namespace osgi

#endif // OSGI_INVALIDSYNTAXEXCEPTION_H_
