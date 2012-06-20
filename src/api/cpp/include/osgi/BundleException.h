
#pragma once
#ifndef OSGI_BUNDLEEXCEPTION_H_
#define OSGI_BUNDLEEXCEPTION_H_

#include <osgi/Exception.h>

namespace osgi {

/**
 * \ingroup cpp_api
 *
 * A Framework exception used to indicate that a bundle lifecycle problem
 * occurred.
 *
 * <p>
 * A \c BundleException object is created by the Framework to denote
 * an exception condition in the lifecycle of a bundle.
 * \c BundleExceptions should not be created by bundle developers.
 * A type code is used to identify the exception type for future extendability.
 *
 * <p>
 * This exception conforms to the general purpose exception chaining mechanism.
 */
class OSGI_CPP_EXPORT BundleException : public Exception
{

public:

  enum Type {

    /**
     * No exception type is specified.
     */
    UNSPECIFIED,

    /**
     * The operation was unsupported. This type can be used anywhere a
     * BundleException can be thrown.
     */
    UNSUPPORTED_OPERATION,

    /**
     * The operation was invalid.
     */
    INVALID_OPERATION,

    /**
     * The bundle manifest was in error.
     */
    MANIFEST_ERROR,

    /**
     * The bundle was not resolved.
     */
    RESOLVE_ERROR,

    /**
     * The bundle activator was in error.
     */
    ACTIVATOR_ERROR,

    /**
     * The operation failed to complete the requested lifecycle state change.
     */
    STATECHANGE_ERROR,

    /**
     * The install or update operation failed because another already installed
     * bundle has the same symbolic name and version. This exception type will
     * only occur if the framework is configured to only allow a single bundle
     * to be installed for a given symbolic name and version.
     *
     * @see Constants#FRAMEWORK_BSNVERSION
     */
    DUPLICATE_BUNDLE_ERROR,

    /**
     * The start transient operation failed because the start level of the
     * bundle is greater than the current framework start level
     */
    START_TRANSIENT_ERROR,

    /**
     * The framework received an error while reading the input stream for a
     * bundle.
     */
    READ_ERROR,

    /**
     * A framework hook rejected the operation.
     */
    REJECTED_BY_HOOK
  };

  /**
   * Creates a \c BundleException with the specified message and
   * exception cause.
   *
   * @param msg The associated message.
   * @param cause The cause of this exception.
   */
  BundleException(const std::string& msg, const Exception& cause);

  /**
   * Creates a \c BundleException with the specified message, type
   * and exception cause.
   *
   * @param msg The associated message.
   * @param type The type for this exception.
   * @param cause The cause of this exception.
   */
  BundleException(const std::string& msg, Type type, const Exception& cause);

  /**
   * Creates a \c BundleException with the specified message and
   * type.
   *
   * @param msg The message.
   * @param type The type for this exception.
   */
  BundleException(const std::string& msg, Type type = UNSPECIFIED);

  ~BundleException() throw();

  /**
   * @see Exception::name()
   */
  const char* name() const throw();

  /**
   * @see Exception::clone()
   */
  BundleException* clone() const;

  /**
   * Returns the type for this exception or \c UNSPECIFIED if the
   * type was unspecified or unknown.
   *
   * @return The type of this exception.
   */
  Type getType() const;

private:

  /**
   * Type of bundle exception.
   */
  Type f_Type;
};

}

#endif // OSGI_BUNDLEEXCEPTION_H_
