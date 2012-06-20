
#pragma once
#ifndef OSGI_SERVICEREGISTRATIONBASE_H_
#define OSGI_SERVICEREGISTRATIONBASE_H_

#include <osgi/Config.h>
#include <osgi/Export.h>
#include <osgi/SharedData.h>
#include <osgi/ServiceProperties.h>

namespace osgi {

template<class S> class ServiceReference;

/**
 * \ingroup cpp_support
 *
 * A service registration interface class for framework implementors.
 */
struct ServiceRegistrationBaseImpl : public SharedData
{
  virtual ~ServiceRegistrationBaseImpl();

  virtual ServiceReference<void> getReference(const std::string& interfaceId, bool cpp) const = 0;

  virtual void setProperties(const ServiceProperties& properties) = 0;

  virtual void unregister() = 0;
};

/**
 * \ingroup cpp_api
 *
 * A registered service.
 *
 * <p>
 * The Framework returns a \c ServiceRegistration object when a
 * \c BundleContext.registerService method invocation is successful.
 * The \c ServiceRegistration object is for the private use of the
 * registering bundle and should not be shared with other bundles.
 * <p>
 * The \c ServiceRegistration object may be used to update the
 * properties of the service or to unregister the service.
 *
 * \note This class is provided as public API for low-level service queries only.
 *       In almost all cases you should use ServiceRegistration<S> instead.
 *
 * @see BundleContext#registerService(String[],Object,Dictionary)
 * @threadsafe
 * @noimplement
 */
class ServiceRegistrationBase
{

public:

  ServiceRegistrationBase(ServiceRegistrationBaseImpl* impl);

  /**
   * Releases any resources held or locked by this
   * <code>ServiceRegistration</code> and renders it invalid.
   */
  ServiceRegistrationBase& operator=(int null);

  bool operator<(const ServiceRegistrationBase& o) const;

  bool operator==(const ServiceRegistrationBase& o) const;

  operator bool() const;

  /**
   * Returns a \c ServiceReference object for a service being
   * registered.
   * <p>
   * The \c ServiceReference object may be shared with other
   * bundles.
   *
   * @throws IllegalStateException If this
   *         \c ServiceRegistration object has already been
   *         unregistered.
   * @return \c ServiceReference object.
   */
  ServiceReference<void> getReference(const std::string& interfaceId, bool cpp = true) const;

  /**
   * Updates the properties associated with a service.
   *
   * <p>
   * The {@link Constants#OBJECTCLASS} and {@link Constants#SERVICE_ID} keys
   * cannot be modified by this method. These values are set by the Framework
   * when the service is registered in the OSGi environment.
   *
   * <p>
   * The following steps are required to modify service properties:
   * <ol>
   * <li>The service's properties are replaced with the provided properties.
   * <li>A service event of type {@link ServiceEvent#MODIFIED} is fired.
   * </ol>
   *
   * @param properties The properties for this service. See {@link Constants}
   *        for a list of standard service property keys. Changes should not
   *        be made to this object after calling this method. To update the
   *        service's properties this method should be called again.
   *
   * @throws IllegalStateException If this \c ServiceRegistration
   *         object has already been unregistered.
   * @throws IllegalArgumentException If \c properties contains
   *         case variants of the same key name.
   */
  void setProperties(const ServiceProperties& properties);

  /**
   * Unregisters a service. Remove a \c ServiceRegistration object
   * from the Framework service registry. All \c ServiceReference
   * objects associated with this \c ServiceRegistration object
   * can no longer be used to interact with the service once unregistration is
   * complete.
   *
   * <p>
   * The following steps are required to unregister a service:
   * <ol>
   * <li>The service is removed from the Framework service registry so that
   * it can no longer be obtained.
   * <li>A service event of type {@link ServiceEvent#UNREGISTERING} is fired
   * so that bundles using this service can release their use of the service.
   * Once delivery of the service event is complete, the
   * \c ServiceReference objects for the service may no longer be
   * used to get a service object for the service.
   * <li>For each bundle whose use count for this service is greater than
   * zero: <br>
   * The bundle's use count for this service is set to zero. <br>
   * If the service was registered with a {@link ServiceFactory} object, the
   * \c ServiceFactory.ungetService method is called to release
   * the service object for the bundle.
   * </ol>
   *
   * @throws IllegalStateException If this
   *         \c ServiceRegistration object has already been
   *         unregistered.
   * @see BundleContext#ungetService
   * @see ServiceFactory#ungetService
   */
  void unregister();

private:

  ExplicitlySharedDataPointer<ServiceRegistrationBaseImpl> d;

};

} // end namespace osgi

#endif // OSGI_SERVICEREGISTRATIONBASE_H_
