
#pragma once
#ifndef OSGI_SERVICEREFERENCE_H_
#define OSGI_SERVICEREFERENCE_H_

#include <osgi/ServiceReferenceBase.h>

namespace osgi {

/**
 * \ingroup cpp_api
 *
 * A reference to a service.
 * 
 * <p>
 * The Framework returns \c ServiceReference objects from the
 * \c BundleContext.getServiceReference and
 * \c BundleContext.getServiceReferences methods.
 * <p>
 * A \c ServiceReference object may be shared between bundles and can
 * be used to examine the properties of the service and to get the service
 * object.
 * <p>
 * Every service registered in the Framework has a unique
 * \c ServiceRegistration object and may have multiple, distinct
 * \c ServiceReference objects referring to it.
 * \c ServiceReference objects associated with a
 * \c ServiceRegistration object have the same \c hashCode
 * and are considered equal (more specifically, their \c equals()
 * method will return \c true when compared).
 * <p>
 * If the same service object is registered multiple times,
 * \c ServiceReference objects associated with different
 * \c ServiceRegistration objects are not equal.
 * 
 * @tparam S Type of Service.
 * @see BundleContext#getServiceReference
 * @see BundleContext#getServiceReferences
 * @see BundleContext#getService
 * @threadsafe
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
 * \ingroup cpp_support
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
