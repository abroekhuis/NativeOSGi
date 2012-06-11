
#pragma once
#ifndef BUNDLECONTEXTIMPL_H_
#define BUNDLECONTEXTIMPL_H_

#include <osgi/BundleException.h>
#include <osgi/BundleContext.h>


namespace osgi {
class ServiceRegistrationBase;
}

class BundleImpl;
class ServiceRegistry; // remove later
typedef ServiceRegistry FrameworkImpl; // Mock up

class BundleContextImpl : public osgi::BundleContext
{

private:

  //Logger m_logger = null;
  FrameworkImpl* m_fw;
  BundleImpl* m_bundle;
  bool m_valid;

  void checkValidity() const;

  osgi::ServiceReferenceBase getBestServiceReference(const std::vector<osgi::ServiceReferenceBase>& refs, bool cppOnly);


  // ------------ BundleContext implementation ---------------

  virtual void* getService(const osgi::ServiceReferenceBase& ref);

  virtual std::vector<osgi::ServiceReferenceBase> getServiceReferences(const std::string& clazz,
                                                                       const std::string& filter);

  virtual osgi::ServiceReferenceBase getServiceReference(const std::string& clazz, bool cppOnly = true);

  virtual osgi::ServiceRegistrationBase registerService(const std::map<std::string,void*>& servicePointers, bool isNativeCpp,
                                                        const osgi::ServiceProperties& properties);

public:

  BundleContextImpl(/*Logger logger, */FrameworkImpl* fw, BundleImpl* bundle);

  void invalidate();

  void addRequirement(const std::string& s);

  void removeRequirement();

  void addCapability();

  void removeCapability();

  std::string getProperty(const std::string& name) const;

  virtual osgi::Bundle* getBundle() const;

  osgi::Filter createFilter(const std::string& expr) const;

  osgi::Bundle* installBundle(const std::string& location, std::istream* is = 0);

  osgi::Bundle* getBundle(long id) const;

  osgi::Bundle* getBundle(const std::string& location) const;

  std::vector<osgi::Bundle*> getBundles() const;

  void addBundleListener(const osgi::BundleListener& l);

  void removeBundleListener(const osgi::BundleListener& l);

  void addServiceListener(const osgi::ServiceListener& l, const std::string& s = std::string());

  void removeServiceListener(const osgi::ServiceListener& l);

  void addFrameworkListener(const osgi::FrameworkListener& l);

  void removeFrameworkListener(const osgi::FrameworkListener& l);

  bool ungetService(const osgi::ServiceReferenceBase& ref) const;

  std::string getDataFile(const std::string& s) const;

};

#endif // BUNDLECONTEXTIMPL_H_
