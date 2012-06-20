
#pragma once
#ifndef OSGI_CONSTANTS_H_
#define OSGI_CONSTANTS_H_

#include <string>

namespace osgi {

/**
 * \ingroup cpp_api
 *
 * Defines standard names for the OSGi environment system properties, service
 * properties, and Manifest header attribute keys.
 *
 * <p>
 * The values associated with these keys are of type \c std::string, unless
 * otherwise indicated.
 *
 * @noimplement
 */
struct Constants
{
  /*
   * Service properties.
   */

  /**
   * Service property identifying all of the class names under which a service
   * was registered in the Framework. The value of this property must be of
   * type \c std::vector<std::string>.
   *
   * <p>
   * This property is set by the Framework when a service is registered.
   */
  static const std::string& OBJECTCLASS(); // = "objectClass";

  /**
   * Service property identifying all of the class names under which a service
   * binding was registered in the Framework. The value of this property must be of
   * type \c std::vector<std::string>.
   *
   * <p>
   * This property is set by the Framework when a service is registered and all
   * available C or C++ bindings have been created.
   */
  static const std::string& OBJECTCLASS_BINDINGS(); // = "objectClass.bindings";

  /**
   * Service property identifying a service's registration number. The value
   * of this property must be of type \c long int.
   *
   * <p>
   * The value of this property is assigned by the Framework when a service is
   * registered. The Framework assigns a unique value that is larger than all
   * previously assigned values since the Framework was started. These values
   * are NOT persistent across restarts of the Framework.
   */
  static const std::string& SERVICE_ID(); // = "service.id";

  /**
   * Service property identifying a service's persistent identifier.
   *
   * <p>
   * This property may be supplied in the \c properties
   * \c Dictionary object passed to the
   * \c BundleContext.registerService method. The value of this property
   * must be of type \c std::string or \c std::vector<std::string>.
   *
   * <p>
   * A service's persistent identifier uniquely identifies the service and
   * persists across multiple Framework invocations.
   *
   * <p>
   * By convention, every bundle has its own unique name space, starting with
   * the bundle's identifier (see {@link Bundle#getBundleId}) and followed by
   * a dot (.). A bundle may use this as the prefix of the persistent
   * identifiers for the services it registers.
   */
  static const std::string& SERVICE_PID(); // = "service.pid";

  /**
   * Service property identifying a service's ranking number.
   *
   * <p>
   * This property may be supplied in the {@code properties
   * Dictionary} object passed to the \c BundleContext.registerService
   * method. The value of this property must be of type \c Integer.
   *
   * <p>
   * The service ranking is used by the Framework to determine the <i>natural
   * order</i> of services, see {@link ServiceReference#compareTo}, and the
   * <i>default</i> service to be returned from a call to the
   * {@link BundleContext#getServiceReference} method.
   *
   * <p>
   * The default ranking is zero (0). A service with a ranking of
   * \c Integer.MAX_VALUE is very likely to be returned as the default
   * service, whereas a service with a ranking of \c Integer.MIN_VALUE is
   * very unlikely to be returned.
   *
   * <p>
   * If the supplied property value is not of type \c int, it is
   * deemed to have a ranking value of zero.
   */
  static const std::string& SERVICE_RANKING(); //  = "service.ranking";

  /**
   * Service property identifying a service's vendor.
   *
   * <p>
   * This property may be supplied in the properties \c Dictionary object
   * passed to the \c BundleContext.registerService method.
   */
  static const std::string& SERVICE_VENDOR(); // = "service.vendor";

  /**
   * Service property identifying a service's description.
   *
   * <p>
   * This property may be supplied in the properties \c Dictionary object
   * passed to the \c BundleContext.registerService method.
   */
  static const std::string& SERVICE_DESCRIPTION(); // = "service.description";

};

} // end namespace osgi

#endif // OSGI_CONSTANTS_H_
