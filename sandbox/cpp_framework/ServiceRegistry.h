#ifndef SERVICEREGISTRY_H_
#define SERVICEREGISTRY_H_


#include <osgi/Threads.h>
#include <osgi/ServiceProperties.h>

#include <osgi/BindingMacros.h>

#include <string>
#include <map>
#include <set>
#include <list>
#include <iostream>

#include <typeinfo>
#include <dlfcn.h>

namespace osgi {
struct Bundle;
struct BundleContext;
class Filter;
template<class S> class ServiceReference;
class ServiceReferenceBase;
class ServiceRegistrationBase;
}


/*
 * A service instance is always registered as a void pointer to an abstract C data type (ADT).
 * By convention, the first element of such a service interface ADT is a
 * member pointing to a service implementation specific data type which
 * usually contains implementation specific data members:
 *
 * struct service_instance {
 *   service_data* data;                                      <-- points to some opaque data handle
 *   void (*service_method_one)(service_data* data, <args>);  <-- function pointers
 *   ...
 * };
 *
 * In the case of a C++ service interface registration, a special service_data struct implementation
 * (provided by the interface bundle) is used to wrap the C++ class pointer:
 *
 * struct cpp_service_data {
 *   void* cpp_handle;
 * };
 *
 * The internally registered abstract C data type then looks like this:
 *
 * struct cpp_service_instance {
 *   cpp_service_data* data;
 *   void (*service_method_one)(cpp_service_data* data, <args>);  <-- optional, only if access through the C interface is implemented
 * };
 *
 *
 */
class ServiceRegistry : public OSGI_DEFAULT_THREADING<ServiceRegistry>
{

public:

  class InterfaceBindingRegistry;

private:

  class UsageCount;

  static ServiceRegistry *instance();

  /**
   * Utility method to retrieve the specified bundle's usage count for the
   * specified service reference.
   * @param bundle The bundle whose usage counts are being searched.
   * @param ref The service reference to find in the bundle's usage counts.
   * @return The associated usage count or null if not found.
   **/
  UsageCount* getUsageCount(osgi::Bundle* bundle, const osgi::ServiceReferenceBase& ref) const;

  /**
   * Utility method to update the specified bundle's usage count array to
   * include the specified service. This method should only be called
   * to add a usage count for a previously unreferenced service. If the
   * service already has a usage count, then the existing usage count
   * counter simply needs to be incremented.
   * @param bundle The bundle acquiring the service.
   * @param ref The service reference of the acquired service.
   * @param svcObj The service object of the acquired service.
   **/
  UsageCount* addUsageCount(osgi::Bundle* bundle, const osgi::ServiceReferenceBase& ref);

  /**
   * Utility method to flush the specified bundle's usage count for the
   * specified service reference. This should be called to completely
   * remove the associated usage count object for the specified service
   * reference. If the goal is to simply decrement the usage, then get
   * the usage count and decrement its counter. This method will also
   * remove the specified bundle from the "in use" map if it has no more
   * usage counts after removing the usage count for the specified service
   * reference.
   * @param bundle The bundle whose usage count should be removed.
   * @param ref The service reference whose usage count should be removed.
   **/
  void flushUsageCount(osgi::Bundle* bundle, const osgi::ServiceReferenceBase& ref);

  // Logger m_logger;
  long m_currentServiceId;

  InterfaceBindingRegistry* m_bindingRegistry;

  osgi::Mutex m_regsMapMutex;
  // Maps bundle to an array of service registrations.
  typedef OSGI_UNORDERED_MAP_TYPE<osgi::Bundle*, std::list<osgi::ServiceRegistrationBase> > BundleToRegsMapType;
  /*__attribute__((guarded_by(m_regsMapMutex)))*/ BundleToRegsMapType m_regsMap;

  // Capability set for all service registrations.
  //CapabilitySet m_regCapSet;

  // Maps registration to thread to keep track when a
  // registration is in use, which will cause other
  // threads to wait.
  //Map m_lockedRegsMap = new HashMap();
  // Maps bundle to an array of usage counts.
  typedef std::list<UsageCount*> InUseMapValueType;
  typedef OSGI_UNORDERED_MAP_TYPE<osgi::Bundle*, InUseMapValueType> InUseMapType;
  InUseMapType m_inUseMap;

public:

  ServiceRegistry();
  ~ServiceRegistry();

  static osgi::BundleContext* bundleContext();

  static osgi_binding_registry* bindingRegistry(const std::string& bundle);

  void *getService(osgi::Bundle* bundle, const osgi::ServiceReferenceBase& ref);

  std::vector<osgi::ServiceReference<void> > getServiceReferences(const std::string& className,
                                                                  const osgi::Filter& filter);

  // Caller is expected to fire REGISTERED event.
  osgi::ServiceRegistrationBase registerService(osgi::Bundle* bundle,
                                                const std::map<std::string, void*> &servicePointers, bool isNativeCpp,
                                                const osgi::ServiceProperties& dict);

  void servicePropertiesModified(const osgi::ServiceRegistrationBase& reg,
                                 const osgi::ServiceProperties& oldProps);

  void unregisterService(osgi::Bundle* bundle, const osgi::ServiceRegistrationBase& reg);

  /**
   * This method retrieves all services registrations for the specified
   * bundle and invokes <tt>ServiceRegistration.unregister()</tt> on each
   * one. This method is only called be the framework to clean up after
   * a stopped bundle.
   * @param bundle the bundle whose services should be unregistered.
   **/
  void unregisterServices(osgi::Bundle* bundle);

  void getUsingBundles(std::set<osgi::Bundle *> &result, const osgi::ServiceReferenceBase& ref);

};

#endif // SERVICEREGISTRY_H_
