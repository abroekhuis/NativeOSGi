
#pragma once
#ifndef SERVICEREGISTRATIONIMPL_H_
#define SERVICEREGISTRATIONIMPL_H_

#include <osgi/ServiceProperties.h>
#include <osgi/ServiceRegistrationBase.h>

#include "ServiceReferenceImpl.h"

#include <vector>
#include <map>

class ServiceRegistry;
struct ServiceHandle;

namespace osgi {
struct Bundle;
}

class ServiceRegistrationImpl : public osgi::ServiceRegistrationBaseImpl,
    private OSGI_DEFAULT_THREADING<ServiceRegistrationImpl>
{

public:

  enum ServiceType { NativeC, NativeCpp };

  ServiceRegistrationImpl(ServiceRegistry* registry, osgi::Bundle* bundle,
      const std::map<std::string,ServiceHandle>& svcHandles, ServiceType svcType,
      long int serviceId, const osgi::ServiceProperties& dict);

  osgi::ServiceReference<void> getReference(const std::string& interfaceId, bool cpp) const;

  void setProperties(const osgi::ServiceProperties& dict);

  void unregister();

  osgi::Any getProperty(const std::string& key) const;

  std::vector<std::string> getPropertyKeys() const;

  std::vector<osgi::Bundle*> getUsingBundles() const;

private:

  friend class ServiceRegistry;

  bool isValid() const;

  bool isValid_unlocked() const;

  void invalidate();

  //
  // Utility methods.
  //

  /**
   * This method provides direct access to the associated service object;
   * it generally should not be used by anyone other than the service registry
   * itself.
   * @return The service object associated with the registration.
   **/
  ServiceHandle getService(const std::string &interfaceId) const;

  ServiceHandle getService(osgi::Bundle* acqBundle, const std::string &interfaceId);

  void ungetService(osgi::Bundle* relBundle, void* svcObj);

  void initializeProperties(const osgi::ServiceProperties& dict);

  void* getFactoryUnchecked(osgi::Bundle* bundle) const;

  void ungetFactoryUnchecked(osgi::Bundle* bundle, void* svcObj);

private:

  // Service registry.
  ServiceRegistry* const m_registry;
  // Bundle providing the service.
  osgi::Bundle* const m_bundle;
  // Service Id associated with the service object.
  const long m_serviceId;
  // Service object handles pointing to all implemented interfaces
  std::map<std::string, ServiceHandle> m_svcHandles;
  // An array of implemented interface names (C implementation/binding)
  std::vector<std::string> m_cInterfaces;
  // An array of implemented interface names (C++ implementation/binding)
  std::vector<std::string> m_cppInterfaces;
  // Stores the native type of the service
  ServiceType m_svcType;
  // Service factory interface.
  //ServiceFactory m_factory;
  // Associated property dictionary.
  osgi::ServiceProperties m_propMap;
  // Re-usable service references.
  std::map<std::string, osgi::ServiceReference<void> > m_cRefs;
  std::map<std::string, osgi::ServiceReference<void> > m_cppRefs;
  // Flag indicating that we are unregistering.
  bool m_isUnregistering;
};

#endif // SERVICEREGISTRATIONIMPL_H_
