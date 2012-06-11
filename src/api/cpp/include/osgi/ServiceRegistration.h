
#pragma once
#ifndef OSGI_SERVICEREGISTRATION_H_
#define OSGI_SERVICEREGISTRATION_H_

#include <osgi/ServiceRegistrationBase.h>

namespace osgi {

template<class S> class ServiceReference;

#ifndef _ServiceRegistrationTemplate_forward_declared_
#define _ServiceRegistrationTemplate_forward_declared_
template<class S, class T = void> class ServiceRegistration;
#endif

template<class Interface>
struct InterfaceType {};

/**
 * A registered service.
 *
 * <p>
 * The Framework returns a {@code ServiceRegistration} object when a
 * {@code BundleContext.registerService} method invocation is successful.
 * The {@code ServiceRegistration} object is for the private use of the
 * registering bundle and should not be shared with other bundles.
 * <p>
 * The {@code ServiceRegistration} object may be used to update the
 * properties of the service or to unregister the service.
 *
 * @param <S> Type of Service.
 * @see BundleContext#registerService(String[],Object,Dictionary)
 * @ThreadSafe
 * @noimplement
 */
template<class S, class T>
class ServiceRegistration : public ServiceRegistrationBase
{

private:

  friend struct BundleContext;

  ServiceRegistration(const ServiceRegistrationBase& regBase)
    : ServiceRegistrationBase(regBase)
  {}

public:

  /**
   * Returns a {@code ServiceReference} object for a service being
   * registered.
   * <p>
   * The {@code ServiceReference} object may be shared with other
   * bundles.
   *
   * @throws IllegalStateException If this
   *         {@code ServiceRegistration} object has already been
   *         unregistered.
   * @return {@code ServiceReference} object.
   */
  ServiceReference<S> getReference(InterfaceType<S>) const
  {
    return ServiceReference<S>(this->getReferenceBase());
  }

  ServiceReference<T> getReference(InterfaceType<T>) const
  {
    return ServiceReference<T>(this->getReferenceBase());
  }

};

template<class S>
class ServiceRegistration<S, void> : public ServiceRegistrationBase
{

private:

  friend struct BundleContext;

  ServiceRegistration(const ServiceRegistrationBase& regBase)
    : ServiceRegistrationBase(regBase)
  {}

public:

  ServiceReference<S> getReference() const
  {
    return this->getReference(InterfaceType<S>());
  }

  ServiceReference<S> getReference(InterfaceType<S>) const
  {
    return ServiceReference<S>(this->getReferenceBase());
  }

};

} // end namespace osgi

#endif // OSGI_SERVICEREGISTRATION_H_
