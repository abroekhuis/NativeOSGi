

#include "ServiceReferenceImpl.h"

#include "ServiceRegistrationImpl.h"

#include <osgi/Any.h>

ServiceReferenceImpl::ServiceReferenceImpl(const std::string &interfaceId, bool isCpp,
                                           ServiceRegistrationImpl* serviceReg)
  : f_InterfaceId(interfaceId), f_ServiceRegistration(serviceReg),
    f_IsCpp(isCpp)
{
}

ServiceRegistrationImpl* ServiceReferenceImpl::getRegistration() const
{
  return f_ServiceRegistration;
}

std::string ServiceReferenceImpl::getInterfaceId() const
{
  return f_InterfaceId;
}

bool ServiceReferenceImpl::isCppReference() const
{
  return f_IsCpp;
}

osgi::Any ServiceReferenceImpl::getProperty(const std::string& s) const
{
  return f_ServiceRegistration->getProperty(s);
}

std::vector<std::string> ServiceReferenceImpl::getPropertyKeys() const
{
  return f_ServiceRegistration->getPropertyKeys();
}

osgi::Bundle* ServiceReferenceImpl::getBundle() const
{
  // The spec says that this should return null if
  // the service is unregistered.
  //return (isValid()) ? m_bundle : null;
  return 0;
}

std::vector<osgi::Bundle*> ServiceReferenceImpl::getUsingBundles() const
{
  return f_ServiceRegistration->getUsingBundles();
}

std::string ServiceReferenceImpl::toString() const
{
//  String[] ocs = (String[]) getProperty("objectClass");
//  String oc = "[";
//  for(int i = 0; i < ocs.length; i++)
//  {
//    oc = oc + ocs[i];
//    if (i < ocs.length - 1)
//      oc = oc + ", ";
//  }
//  oc = oc + "]";
//  return oc;
  return "ServiceReferenceImpl::toString() not implemented yet";
}

std::size_t ServiceReferenceImpl::hash() const
{
  OSGI_USING_HASH_FUNCTION;
  return OSGI_HASH_FUNCTION(ServiceRegistrationImpl*, f_ServiceRegistration);
}
