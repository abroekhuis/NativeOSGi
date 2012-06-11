
#include <osgi/ServiceInterface.h>
#include <osgi/ServiceException.h>
#include <osgi/ServiceRegistration.h>
#include <osgi/ServiceReferenceBase.h>

#include <typeinfo>

namespace osgi {

template<class S>
ServiceRegistration<S> BundleContext::registerService(S* service, const ServiceProperties& properties)
{
  const char* clazz = osgi_service_interface_id<S*>();
  if (clazz == 0)
  {
    throw ServiceException(std::string("The interface class you are registering your service ") +
                           typeid(S).name() + " against has no OSGI_DECLARE_SERVICE_INTERFACE macro");
  }
  std::map<std::string,void*> servicePointers;
  servicePointers.insert(std::make_pair(std::string(clazz), static_cast<void*>(service)));
  return ServiceRegistration<S>(registerService(servicePointers, true, properties));
}

template<class S>
std::vector<ServiceReference<S> > BundleContext::getServiceReferences(const std::string& filter)
{
  const char* clazz = osgi_service_interface_id<S*>();
  if (clazz == 0) throw ServiceException("The service interface class has no OSGI_DECLARE_SERVICE_INTERFACE macro");
  typedef std::vector<ServiceReferenceBase> BaseVectorT;
  BaseVectorT serviceRefs = getServiceReferences(std::string(clazz), filter);
  std::vector<ServiceReference<S> > result;
  for(BaseVectorT::const_iterator i = serviceRefs.begin(); i != serviceRefs.end(); ++i)
  {
    if (i->isCppReference())
    {
      result.push_back(ServiceReference<S>(*i));
    }
  }
  return result;
}

template<class S>
ServiceReference<S> BundleContext::getServiceReference()
{
  const char* clazz = osgi_service_interface_id<S*>();
  if (clazz == 0) throw ServiceException("The service interface class has no OSGI_DECLARE_SERVICE_INTERFACE macro");
  return ServiceReference<S>(getServiceReference(std::string(clazz), true));
}

template<class S>
S* BundleContext::getService(const ServiceReference<S>& reference)
{
  return reinterpret_cast<S*>(getService(static_cast<const ServiceReferenceBase&>(reference)));
}

template<class R>
void BundleContext::addServiceListener(R* receiver, void(R::*callback)(const ServiceEvent),
                                       const std::string& filter)
{
  addServiceListener(ServiceListenerMemberFunctor(receiver, callback), filter);
}

template<class R>
void BundleContext::removeServiceListener(R* receiver, void(R::*callback)(const ServiceEvent))
{
  removeServiceListener(ServiceListenerMemberFunctor(receiver, callback));
}

template<class R>
void BundleContext::addBundleListener(R* receiver, void(R::*callback)(const BundleEvent))
{
  addBundleListener(BundleListenerMemberFunctor(receiver, callback));
}

template<class R>
void BundleContext::removeBundleListener(R* receiver, void(R::*callback)(const BundleEvent))
{
  removeBundleListener(BundleListenerMemberFunctor(receiver, callback));
}

} // end namespace osgi
