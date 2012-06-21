
#pragma once
#ifndef OSGI_SERVICEREFERENCE_H_
#define OSGI_SERVICEREFERENCE_H_

#include <osgi/ServiceReferenceBase.h>

namespace osgi {

/**
 * A reference to a service.
 * 
 * <p>
 * The Framework returns {@code ServiceReference} objects from the
 * {@code BundleContext.getServiceReference} and
 * {@code BundleContext.getServiceReferences} methods.
 * <p>
 * A {@code ServiceReference} object may be shared between bundles and can
 * be used to examine the properties of the service and to get the service
 * object.
 * <p>
 * Every service registered in the Framework has a unique
 * {@code ServiceRegistration} object and may have multiple, distinct
 * {@code ServiceReference} objects referring to it.
 * {@code ServiceReference} objects associated with a
 * {@code ServiceRegistration} object have the same {@code hashCode}
 * and are considered equal (more specifically, their {@code equals()}
 * method will return {@code true} when compared).
 * <p>
 * If the same service object is registered multiple times,
 * {@code ServiceReference} objects associated with different
 * {@code ServiceRegistration} objects are not equal.
 * 
 * @param <S> Type of Service.
 * @see BundleContext#getServiceReference
 * @see BundleContext#getServiceReferences
 * @see BundleContext#getService
 * @ThreadSafe
 * @noimplement
 */
template<class S>
class ServiceReference : public ServiceReferenceBase
{

private:

  friend struct BundleContext;
  template<class A, class B> friend class ServiceRegistration;

  ServiceReference(const ServiceReferenceBase& refBase)
    : ServiceReferenceBase(refBase)
  {
  }

public:

  typedef S ServiceType;

};

/**
 * \cond internal
 *
 * Specialization for void. The public constructor allows
 * framework implementations to construct this template
 * specialization based on a ServiceReferenceBase instance.
 *
 */
template<>
class ServiceReference<void> : public ServiceReferenceBase
{

public:

  /**
   * Construct an invalid service reference.
   */
  ServiceReference()
  {}

  ServiceReference(const ServiceReferenceBase& refBase)
    : ServiceReferenceBase(refBase)
  {
  }

  typedef void ServiceType;

};

}

#endif // OSGI_SERVICEREFERENCE_H_
