
#include "ServiceRegistrationImpl.h"

#include "ServiceRegistry.h"
#include "ServiceHandle.h"

#include <osgi/Exception.h>
#include <osgi/Constants.h>
#include <osgi/ServiceReference.h>

ServiceRegistrationImpl::ServiceRegistrationImpl(ServiceRegistry* registry,
    osgi::Bundle* bundle,
    const std::map<std::string,ServiceHandle>& svcHandles,
    ServiceType svcType, long int serviceId,
    const osgi::ServiceProperties& dict
    )
  : m_registry(registry),
    m_bundle(bundle),
    m_svcHandles(svcHandles),
    m_svcType(svcType),
    m_serviceId(serviceId),
    m_isUnregistering(false)
{
  for(std::map<std::string,ServiceHandle>::const_iterator i = svcHandles.begin();
      i != svcHandles.end(); ++i)
  {
    if (i->second.f_CInterface)
    {
      // This reference is the "standard" reference for this
      // service and will always be returned by getReference(std::string).
      osgi::ServiceReferenceBase ref(new ServiceReferenceImpl(i->first, false, this));
      m_cRefs.insert(std::make_pair(i->first, ref));
      m_cInterfaces.push_back(i->first);
    }

    if (i->second.f_CppInterface)
    {
      // This reference is the "standard" reference for this
      // service and will always be returned by getReference(std::string).
      osgi::ServiceReferenceBase ref(new ServiceReferenceImpl(i->first, true, this));
      m_cppRefs.insert(std::make_pair(i->first, ref));
      m_cppInterfaces.push_back(i->first);
    }
  }

  //     m_factory = (m_svcObj instanceof ServiceFactory)
  //       ? (ServiceFactory) m_svcObj : null;

  initializeProperties(dict);
}

osgi::ServiceReference<void> ServiceRegistrationImpl::getReference(const std::string& interfaceId, bool cpp) const
{
  Lock l(*this);
  // Make sure registration is valid.
  if (!isValid_unlocked())
  {
    throw osgi::IllegalStateException(
          "The service registration is no longer valid.");
  }

  if (cpp)
  {
    std::map<std::string, osgi::ServiceReference<void> >::const_iterator i = m_cppRefs.find(interfaceId);
    if (i != m_cppRefs.end()) return i->second;
  }
  else
  {
    std::map<std::string, osgi::ServiceReference<void> >::const_iterator i = m_cRefs.find(interfaceId);
    if (i != m_cRefs.end()) return i->second;
  }
  return osgi::ServiceReference<void>();
}

void ServiceRegistrationImpl::setProperties(const osgi::ServiceProperties& dict)
{
  osgi::ServiceProperties oldProps;
  {
    Lock l(*this);
    // Make sure registration is valid.
    if (!isValid_unlocked())
    {
      throw osgi::IllegalStateException(
            "The service registration is no longer valid.");
    }
    // Remember old properties.
    oldProps = m_propMap;
    // Set the properties.
    initializeProperties(dict);
  }
  // Tell registry about it.
  m_registry->servicePropertiesModified(this, oldProps);
}

void ServiceRegistrationImpl::unregister()
{
  {
    Lock l(*this);
    if (!isValid_unlocked() || m_isUnregistering)
    {
      throw osgi::IllegalStateException("Service already unregistered.");
    }
    m_isUnregistering = true;
  }
  m_registry->unregisterService(m_bundle, this);
  {
    Lock l(*this);
    m_svcHandles.clear();
    //m_factory = 0;
  }
}

bool ServiceRegistrationImpl::isValid() const
{
  Lock l(*this);
  return isValid_unlocked();
}

bool ServiceRegistrationImpl::isValid_unlocked() const
{
  return (!m_svcHandles.empty());
}

void ServiceRegistrationImpl::invalidate()
{
  Lock l(*this);
  m_svcHandles.clear();
}

osgi::Any ServiceRegistrationImpl::getProperty(const std::string& key) const
{
  osgi::ServiceProperties::const_iterator it = m_propMap.find(key);
  if (it != m_propMap.end())
  {
    return *it;
  }
  return osgi::Any();
}

std::vector<std::string> ServiceRegistrationImpl::getPropertyKeys() const
{
  std::vector<std::string> keys;
  for (osgi::ServiceProperties::const_iterator it = m_propMap.begin();
       it != m_propMap.end(); ++it)
  {
    keys.push_back(it->first);
  }
  return keys;
}

std::vector<osgi::Bundle*> ServiceRegistrationImpl::getUsingBundles() const
{
  std::set<osgi::Bundle*> result;
  m_registry->getUsingBundles(result, m_cRefs.begin()->second);
  m_registry->getUsingBundles(result, m_cppRefs.begin()->second);
  return std::vector<osgi::Bundle*>(result.begin(), result.end());
}

ServiceHandle ServiceRegistrationImpl::getService(const std::string& interfaceId) const
{
  std::map<std::string,ServiceHandle>::const_iterator i = m_svcHandles.find(interfaceId);
  if (i != m_svcHandles.end()) return i->second;
  ServiceHandle svcHandle = { NULL, NULL };
  return svcHandle;
}

ServiceHandle ServiceRegistrationImpl::getService(osgi::Bundle* acqBundle, const std::string& interfaceId)
{
//    // If the service object is a service factory, then
//    // let it create the service object.
//    if (m_factory != null)
//    {
//      Object svcObj = null;
//      try
//      {
//        if (System.getSecurityManager() != null)
//        {
//          svcObj = AccessController.doPrivileged(
//                new ServiceFactoryPrivileged(acqBundle, null));
//        }
//        else
//        {
//          svcObj = getFactoryUnchecked(acqBundle);
//        }
//      }
//      catch (PrivilegedActionException ex)
//      {
//        if (ex.getException() instanceof ServiceException)
//        {
//          throw (ServiceException) ex.getException();
//        }
//        else
//        {
//          throw new ServiceException(
//                "Service factory exception: " + ex.getException().getMessage(),
//                ServiceException.FACTORY_EXCEPTION, ex.getException());
//        }
//      }
//      return svcObj;
//    }
//    else
  {
    std::map<std::string,ServiceHandle>::const_iterator i = m_svcHandles.find(interfaceId);
    if (i != m_svcHandles.end()) return i->second;
    ServiceHandle svcHandle = { NULL, NULL };
    return svcHandle;
  }
}

void ServiceRegistrationImpl::ungetService(osgi::Bundle* relBundle, void* svcObj)
{
//    // If the service object is a service factory, then
//    // let it release the service object.
//    if (m_factory != null)
//    {
//      try
//      {
//        if (System.getSecurityManager() != null)
//        {
//          AccessController.doPrivileged(
//                new ServiceFactoryPrivileged(relBundle, svcObj));
//        }
//        else
//        {
//          ungetFactoryUnchecked(relBundle, svcObj);
//        }
//      }
//      catch (Exception ex)
//      {
//        m_registry.getLogger().log(
//              m_bundle,
//              Logger.LOG_ERROR,
//              "ServiceRegistrationImpl: Error ungetting service.",
//              ex);
//      }
//    }
}

void ServiceRegistrationImpl::initializeProperties(const osgi::ServiceProperties& dict)
{
  m_propMap = dict;

  // Add the framework assigned properties.
  if (m_svcType == NativeC)
  {
    m_propMap.insert(std::make_pair(osgi::Constants::OBJECTCLASS(), m_cInterfaces));
    m_propMap.insert(std::make_pair(osgi::Constants::OBJECTCLASS_BINDINGS(), m_cppInterfaces));
  }
  else
  {
    m_propMap.insert(std::make_pair(osgi::Constants::OBJECTCLASS(), m_cppInterfaces));
    m_propMap.insert(std::make_pair(osgi::Constants::OBJECTCLASS_BINDINGS(), m_cInterfaces));
  }
  m_propMap.insert(std::make_pair(osgi::Constants::SERVICE_ID(), m_serviceId));
}

void* ServiceRegistrationImpl::getFactoryUnchecked(osgi::Bundle* bundle) const
{
  void* svcObj = NULL;
//  try
//  {
//    svcObj = m_factory.getService(bundle, this);
//  }
//  catch (Throwable th)
//  {
//    throw new ServiceException(
//          "Service factory exception: " + th.getMessage(),
//          ServiceException.FACTORY_EXCEPTION, th);
//  }
//  if (svcObj != null)
//  {
//    for (int i = 0; i < m_classes.length; i++)
//    {
//      Class clazz = Util.loadClassUsingClass(
//            svcObj.getClass(), m_classes[i], Felix.m_secureAction);
//      if ((clazz == null) || !clazz.isAssignableFrom(svcObj.getClass()))
//      {
//        if (clazz == null)
//        {
//          throw new ServiceException(
//                "Service cannot be cast due to missing class: " + m_classes[i],
//                ServiceException.FACTORY_ERROR);
//        }
//        else
//        {
//          throw new ServiceException(
//                "Service cannot be cast: " + m_classes[i],
//                ServiceException.FACTORY_ERROR);
//        }
//      }
//    }
//  }
//  else
//  {
//    throw osgi::ServiceException(
//          "Service factory returned null.", ServiceException.FACTORY_ERROR);
//  }
  return svcObj;
}

void ServiceRegistrationImpl::ungetFactoryUnchecked(osgi::Bundle* bundle, void* svcObj)
{
  //m_factory.ungetService(bundle, this, svcObj);
}
