#include "ServiceRegistry.h"

#include "BundleContextImpl.h"
#include "ServiceRegistrationImpl.h"
#include "ServiceHandle.h"

#include <osgi/ServiceReferenceBase.h>


// ------------------ Binding Registry C functions -----------------

void register_c_binding(void* instance, const char* interface_id,
                        osgi_create_c_binding create_c_binding, osgi_destroy_c_binding destroy_c_binding);

void register_cpp_binding(void* instance, const char* interface_id,
                          osgi_create_cpp_binding create_cpp_binding, osgi_destroy_cpp_binding destroy_cpp_binding);

void unregister_c_binding(void* instance, const char* interface_id);

void unregister_cpp_binding(void* instance, const char* interface_id);

// ---------------------- Utility classes --------------------------

struct ServiceRegistry::UsageCount
{
  int m_Count;
  osgi::ServiceReferenceBase m_Ref;
  std::map<std::string, ServiceHandle> m_svcHandles;

  UsageCount() : m_Count(0) {}
};

class ServiceRegistry::InterfaceBindingRegistry
{
  typedef OSGI_UNORDERED_MAP_TYPE<std::string, std::map<std::string, std::pair<osgi_create_c_binding,osgi_destroy_c_binding> > > AllCBindingsMapType;
  AllCBindingsMapType m_AllCBindingsMap;

  typedef OSGI_UNORDERED_MAP_TYPE<std::string, std::pair<osgi_create_c_binding,osgi_destroy_c_binding> > CBindingMapType;
  CBindingMapType m_CBindingMap;

  typedef OSGI_UNORDERED_MAP_TYPE<std::string, std::map<std::string, std::pair<osgi_create_cpp_binding,osgi_destroy_cpp_binding> > > AllCppBindingsMapType;
  AllCppBindingsMapType m_AllCppBindingsMap;

  typedef OSGI_UNORDERED_MAP_TYPE<std::string, std::pair<osgi_create_cpp_binding,osgi_destroy_cpp_binding> > CppBindingMapType;
  CppBindingMapType m_CppBindingMap;

  std::map<std::string, osgi_binding_registry> m_BundleRegistries;

public:

  struct BindingRegistryHandle
  {
    InterfaceBindingRegistry* registry;
    std::string bundle;
  };

  osgi_binding_registry* getBindingRegistry(const std::string& bundle)
  {
    std::map<std::string, osgi_binding_registry>::iterator i = m_BundleRegistries.find(bundle);
    if (i != m_BundleRegistries.end()) return &(i->second);

    BindingRegistryHandle* handle = new BindingRegistryHandle;
    handle->registry = this;
    handle->bundle = bundle;

    osgi_binding_registry registry;
    registry.instance = static_cast<void*>(handle);
    registry.register_c_binding = register_c_binding;
    registry.register_cpp_binding = register_cpp_binding;

    m_BundleRegistries.insert(std::make_pair(bundle, registry));
    return &m_BundleRegistries[bundle];
  }

  void registerCBinding(const std::string& interfaceId, const std::string& bundle,
                        osgi_create_c_binding create_c_binding, osgi_destroy_c_binding destroy_c_binding)
  {
    m_AllCBindingsMap[interfaceId].insert(std::make_pair(bundle, std::make_pair(create_c_binding, destroy_c_binding)));
    m_CBindingMap[interfaceId] = std::make_pair(create_c_binding,destroy_c_binding);
  }

  void unregisterCBinding(const std::string& interfaceId, const std::string& bundle)
  {

  }

  void registerCppBinding(const std::string& interfaceId, const std::string& bundle,
                          osgi_create_cpp_binding create_cpp_binding, osgi_destroy_cpp_binding destroy_cpp_binding)
  {
    m_AllCppBindingsMap[interfaceId].insert(std::make_pair(bundle, std::make_pair(create_cpp_binding, destroy_cpp_binding)));
    m_CppBindingMap[interfaceId] = std::make_pair(create_cpp_binding,destroy_cpp_binding);
  }

  void unregisterCppBinding(const std::string& interfaceId, const std::string& bundle)
  {

  }


  void* createCBinding(const std::string &interfaceId, void* cppInterface) const
  {
    CBindingMapType::const_iterator i = m_CBindingMap.find(interfaceId);
    if (i != m_CBindingMap.end()) return i->second.first(cppInterface);
    return NULL;
  }

  void* createCppBinding(const std::string &interfaceId, void* c_struct) const
  {
    CppBindingMapType::const_iterator i = m_CppBindingMap.find(interfaceId);
    if (i != m_CppBindingMap.end()) return i->second.first(c_struct);
    return NULL;
  }

  void destroyCBinding(const std::string& interfaceId, void* cBinding) const
  {
    CBindingMapType::const_iterator i = m_CBindingMap.find(interfaceId);
    if (i != m_CBindingMap.end()) i->second.second(cBinding);
  }

  void destroyCppBinding(const std::string &interfaceId, void* cppBinding) const
  {
    CppBindingMapType::const_iterator i = m_CppBindingMap.find(interfaceId);
    if (i != m_CppBindingMap.end()) i->second.second(cppBinding);
  }
};

// ------------------ Binding Registry C function implementation -----------------

void register_c_binding(void* instance, const char* interface_id,
                        osgi_create_c_binding create_c_binding, osgi_destroy_c_binding destroy_c_binding)
{
  ServiceRegistry::InterfaceBindingRegistry::BindingRegistryHandle* handle =
      reinterpret_cast<ServiceRegistry::InterfaceBindingRegistry::BindingRegistryHandle*>(instance);
  handle->registry->registerCBinding(interface_id, handle->bundle, create_c_binding, destroy_c_binding);
}

void register_cpp_binding(void* instance, const char* interface_id,
                          osgi_create_cpp_binding create_cpp_binding, osgi_destroy_cpp_binding destroy_cpp_binding)
{
  ServiceRegistry::InterfaceBindingRegistry::BindingRegistryHandle* handle =
      reinterpret_cast<ServiceRegistry::InterfaceBindingRegistry::BindingRegistryHandle*>(instance);
  handle->registry->registerCppBinding(interface_id, handle->bundle, create_cpp_binding, destroy_cpp_binding);
}

// ------------------- Service Registry implementation --------------------

ServiceRegistry::UsageCount* ServiceRegistry::getUsageCount(osgi::Bundle* bundle,
                                                            const osgi::ServiceReferenceBase& ref) const
{
  InUseMapType::const_iterator it = m_inUseMap.find(bundle);
  if (it == m_inUseMap.end()) return NULL;

  for (InUseMapValueType::const_iterator u = it->second.begin(); u != it->second.end(); ++u)
  {
    if ((*u)->m_Ref == ref)
    {
      return *u;
    }
  }
  return NULL;
}

ServiceRegistry::UsageCount* ServiceRegistry::addUsageCount(osgi::Bundle* bundle,
                                                            const osgi::ServiceReferenceBase& ref)
{
    UsageCount* usage = new UsageCount();
    usage->m_Ref = ref;

    m_inUseMap[bundle].push_back(usage);
    return usage;
}

void ServiceRegistry::flushUsageCount(osgi::Bundle* bundle,
                                      const osgi::ServiceReferenceBase& ref)
{
  InUseMapType::iterator it = m_inUseMap.find(bundle);
  if (it == m_inUseMap.end()) return;

  InUseMapValueType& usages = it->second;
  for (InUseMapValueType::iterator i = usages.begin(); i != usages.end(); ++i)
  {
    if ((*i)->m_Ref == ref)
    {
      // Remove the usage count from the list
      delete *i;
      usages.erase(i);
      // If this is the only usage, then remove the list from the map
      if (usages.empty())
      {
        m_inUseMap.erase(it);
      }
      break;
    }
  }
}

ServiceRegistry* ServiceRegistry::instance()
{
  static ServiceRegistry registry;
  return &registry;
}

osgi::BundleContext *ServiceRegistry::bundleContext()
{
  static BundleContextImpl context(instance(), NULL);
  return &context;
}

osgi_binding_registry* ServiceRegistry::bindingRegistry(const std::string &bundle)
{
  ServiceRegistry* registry = instance();
  registry->m_bindingRegistry->getBindingRegistry(bundle);
}

void* ServiceRegistry::getService(osgi::Bundle* bundle, const osgi::ServiceReferenceBase& ref)
{
  UsageCount* usage = NULL;
  void* svcObj = NULL;

  // Get the service registration.
  ServiceRegistrationImpl* reg = dynamic_cast<ServiceReferenceImpl*>((osgi::ServiceReferenceBaseImpl*)ref)->getRegistration();

  {
    Lock(*this);
//    // First make sure that no existing operation is currently
//    // being performed by another thread on the service registration.
//    for (Object o = m_lockedRegsMap.get(reg); (o != null); o = m_lockedRegsMap.get(reg))
//    {
//      // We don't allow cycles when we call out to the service factory.
//      if (o.equals(Thread.currentThread()))
//      {
//        throw osgi::ServiceException(
//              "ServiceFactory.getService() resulted in a cycle.",
//              osgi::ServiceException::FACTORY_ERROR);
//      }

//      // Otherwise, wait for it to be freed.
//      try
//      {
//        wait();
//      }
//      catch (InterruptedException ex)
//      {
//      }
//    }

//    // Lock the service registration.
//    m_lockedRegsMap.put(reg, Thread.currentThread());

    // Make sure the service registration is still valid.
    if (reg->isValid())
    {
      // Get the usage count, if any.
      usage = getUsageCount(bundle, ref);

      // If we don't have a usage count, then create one and
      // since the spec says we increment usage count before
      // actually getting the service object.
      if (usage == NULL)
      {
        usage = addUsageCount(bundle, ref);
      }

      // Increment the usage count and grab the already retrieved
      // service object, if one exists.
      usage->m_Count++;
      svcObj = ref.isCppReference() ? usage->m_svcHandles[ref.getInterfaceId()].f_CppInterface :
                                         usage->m_svcHandles[ref.getInterfaceId()].f_CInterface;
    }
  }

  // If we have a usage count, but no service object, then we haven't
  // cached the service object yet, so we need to create one now without
  // holding the lock, since we will potentially call out to a service
  // factory.
  {
    struct Finalize
    {
      Finalize(ServiceRegistry* registry, ServiceRegistrationImpl* reg, void* const svcObj, osgi::Bundle* bundle,
               const osgi::ServiceReferenceBase& ref, UsageCount* usage)
        : registry(registry), reg(reg), svcObj(svcObj), bundle(bundle), ref(ref), usage(usage)
      {}

      ~Finalize()
      {
        // If we successfully retrieved a service object, then we should
        // cache it in the usage count. If not, we should flush the usage
        // count. Either way, we need to unlock the service registration
        // so that any threads waiting for it can continue.
        {
          Lock l(*registry);
          // Before caching the service object, double check to see if
          // the registration is still valid, since it may have been
          // unregistered while we didn't hold the lock.
          if (!reg->isValid() || (svcObj == NULL))
          {
            registry->flushUsageCount(bundle, ref);
          }
          else
          {
            ref.isCppReference() ? usage->m_svcHandles[ref.getInterfaceId()].f_CppInterface = svcObj :
                usage->m_svcHandles[ref.getInterfaceId()].f_CInterface = svcObj;
          }
          // TODO threading
          //m_lockedRegsMap.remove(reg);
          //notifyAll();
        }
      }

      ServiceRegistry* const registry;
      ServiceRegistrationImpl* const reg;
      void* const svcObj;
      osgi::Bundle* const bundle;
      const osgi::ServiceReferenceBase ref;
      UsageCount* const usage;
    };

    Finalize(this, reg, svcObj, bundle, ref, usage);

    if ((usage != NULL) && (svcObj == NULL))
    {
      svcObj = ref.isCppReference() ? reg->getService(bundle, ref.getInterfaceId()).f_CppInterface :
                                      reg->getService(bundle, ref.getInterfaceId()).f_CInterface;
    }
  }

//  ServiceHandle serviceHandle = getService(interfaceId);
//  S* service = 0;
//  if (serviceHandle.handle)
//  {
//    if (serviceHandle.isCppService)
//    {
//      /*
//       * Short-cut. Consuming a C++ service implementation through its C++ interface
//       * should not go through the C interface (there might not even be one). More
//       * importantly, this avoids argument conversions to C and back to C++ types.
//       */
//      struct instance_handle {
//        void* instance;
//      };
//      instance_handle* instance_h = reinterpret_cast<instance_handle*>(serviceHandle.handle);
//      instance_handle* cppinstance_h = reinterpret_cast<instance_handle*>(instance_h->instance);
//      service = reinterpret_cast<S*>(cppinstance_h->instance);
//    }
//    else
//    {
//      /*
//       * Get the interface specific C++ wrapper for the C struct. We look for a special
//       * symbol, which is usually implemented in the interface bundle but could also
//       * be found in any shared library which is in the symbol look-up path of the
//       * calling library (service consumer).
//       */

//      std::string ctocpp_symbolname = std::string(interfaceId) + "_ctocpp";
//      void* ctocpp_symbol = dlsym(RTLD_DEFAULT, ctocpp_symbolname.c_str());
//      if (ctocpp_symbol == NULL)
//      {
//        // There is no C++ interface wrapper for the C service implementation available
//        std::cerr << "error: " << ctocpp_symbolname << " symbol not found" << std::endl;
//        return NULL;
//      }

//      typedef void* (*ctocpp_handle)(void*);
//      ctocpp_handle ctocpp = reinterpret_cast<ctocpp_handle>(ctocpp_symbol);

//      /**
//       * A call to this function returns a custom C++ interface implementation
//       * mapping the C++ interface method calls to the C functions provided
//       * by this C service implementation.
//       */
//      service = reinterpret_cast<S*>(ctocpp(serviceHandle.handle));
//    }
//  }
//  return service;

  return svcObj;
}

std::vector<osgi::ServiceReferenceBase>
ServiceRegistry::getServiceReferences(const std::string& className, const osgi::Filter& filter)
{
  Lock(*this);
  // TODO filter
  if ((className.empty())) // && (filter.isNull()))
  {
    // Return all services.
    //filter = new SimpleFilter(Constants.OBJECTCLASS, "*", SimpleFilter.PRESENT);
  }
  else if ((!className.empty())) // && (filter.isNull()))
  {
    // Return services matching the class name.
    //filter = new SimpleFilter(Constants.OBJECTCLASS, className, SimpleFilter.EQ);
  }
  else if ((!className.empty())) // && (!filter.isNull()))
  {
//    // Return services matching the class name and filter.
//    List filters = new ArrayList(2);
//    filters.add(new SimpleFilter(Constants.OBJECTCLASS, className, SimpleFilter.EQ));
//    filters.add(filter);
//    filter = new SimpleFilter(null, filters, SimpleFilter.AND);
  }
  // else just use the specified filter.

  //Set<BundleCapability> matches = m_regCapSet.match(filter, false);

  // Quick hack
  std::vector<osgi::ServiceReferenceBase> matches;
  for (BundleToRegsMapType::const_iterator i = m_regsMap.begin(); i != m_regsMap.end(); ++i)
  {
    for (std::list<osgi::ServiceRegistrationBase>::const_iterator reg = i->second.begin();
         reg != i->second.end(); ++reg)
    {
      osgi::ServiceReferenceBase ref = reg->getReference(className, true);
      if (ref) matches.push_back(ref);
      ref = reg->getReference(className, false);
      if (ref) matches.push_back(ref);
    }
  }
  // End quick hack

  return matches;
}

osgi::ServiceRegistrationBase
ServiceRegistry::registerService(osgi::Bundle* bundle, const std::map<std::string,void*>& servicePointers,
                                 bool isNativeCpp, const osgi::ServiceProperties& dict)
{
  osgi::ServiceRegistrationBase reg(0);

  {
    Lock(*this);

    std::map<std::string, ServiceHandle> svcHandles;

    for(std::map<std::string,void*>::const_iterator i = servicePointers.begin();
        i != servicePointers.end(); ++i)
    {
      ServiceHandle svcHandle = { NULL, NULL };
      if (isNativeCpp)
      {
        svcHandle.f_CppInterface = i->second;

        // see if we can get a C binding
        svcHandle.f_CInterface = m_bindingRegistry->createCBinding(i->first, i->second);
      }
      else
      {
        svcHandle.f_CInterface = i->second;

        // see if we can get a C++ binding
        svcHandle.f_CppInterface = m_bindingRegistry->createCppBinding(i->first, i->second);
      }

      svcHandles.insert(std::make_pair(i->first, svcHandle));
    }

    // Create the service registration.
    reg = osgi::ServiceRegistrationBase(new ServiceRegistrationImpl(
                                          this, bundle, svcHandles,
                                          isNativeCpp ? ServiceRegistrationImpl::NativeCpp : ServiceRegistrationImpl::NativeC,
                                          m_currentServiceId++, dict));

    // Keep track of registered hooks.
    //addHooks(classNames, svcObj, reg.getReference());

    // Add the service registration to the bundles current registered services.
    m_regsMap[bundle].push_back(reg);
    //m_regCapSet.addCapability((BundleCapabilityImpl) reg.getReference());
  }

//  // We are registering a C++ service implementation, using the C++ service interface

//  /*
//   * Check if the interface bundle (or any other shared library which is in the symbol
//   * look-up path) provides a C function which creates a corresponding C interface struct which
//   * maps individual C function calls to the C++ service interface methods.
//   */
//  std::string cpptoc_symbolname = std::string(interfaceId) + "_cpptoc";
//  void* cpptoc_symbol = dlsym(RTLD_DEFAULT, cpptoc_symbolname.c_str());
//  //void* cpptoc_symbol = getCppToCSymbol(interfaceId);
//  if (cpptoc_symbol == NULL)
//  {
//    // No interface specific C to C++ mapping available. We just create a generic
//    // C struct which wraps the pointer to the C++ service class instance.

//    std::cout << "warning: " << cpptoc_symbolname << " symbol not found. C++ service will not be available via C." << std::endl;
//    struct instance_handle {
//      void* instance;
//    };
//    instance_handle* instance_h = new instance_handle;
//    instance_handle* cppinstance_h = new instance_handle;
//    cppinstance_h->instance = static_cast<void*>(impl);
//    instance_h->instance = cppinstance_h;
//    // This is a C++ service implementation, having no C interface.
//    registerService(interfaceId, instance_h, true, false);
//  }
//  else
//  {
//    typedef void* (*cpptoc_handle)(void*);
//    cpptoc_handle cpptoc = reinterpret_cast<cpptoc_handle>(cpptoc_symbol);
//    void* c_handle = cpptoc(static_cast<void*>(impl));
//    // This is a C++ service implementation, having a C interface
//    registerService(interfaceId, c_handle, true, true);
//  }

  return reg;
}

void ServiceRegistry::servicePropertiesModified(const osgi::ServiceRegistrationBase& reg,
                               const osgi::ServiceProperties& oldProps)
{
//    if (m_callbacks != null)
//    {
//      m_callbacks.serviceChanged(
//            new ServiceEvent(ServiceEvent.MODIFIED, reg.getReference()), oldProps);
//    }
}

void ServiceRegistry::unregisterService(osgi::Bundle* bundle, const osgi::ServiceRegistrationBase& reg)
{
//    // If this is a hook, it should be removed.
//    removeHook(reg.getReference());

//    synchronized (this)
//    {
//      // Note that we don't lock the service registration here using
//      // the m_lockedRegsMap because we want to allow bundles to get
//      // the service during the unregistration process. However, since
//      // we do remove the registration from the service registry, no
//      // new bundles will be able to look up the service.

//      // Now remove the registered service.
//      ServiceRegistration[] regs = (ServiceRegistration[]) m_regsMap.get(bundle);
//      m_regsMap.put(bundle, removeServiceRegistration(regs, reg));
//      m_regCapSet.removeCapability((BundleCapabilityImpl) reg.getReference());
//    }

//    // Notify callback objects about unregistering service.
//    if (m_callbacks != null)
//    {
//      m_callbacks.serviceChanged(
//            new ServiceEvent(ServiceEvent.UNREGISTERING, reg.getReference()), null);
//    }

//    // Now forcibly unget the service object for all stubborn clients.
//    synchronized (this)
//    {
//      Bundle[] clients = getUsingBundles(reg.getReference());
//      for (int i = 0; (clients != null) && (i < clients.length); i++)
//      {
//        while (ungetService(clients[i], reg.getReference()))
//          ; // Keep removing until it is no longer possible
//      }
//      ((ServiceRegistrationImpl) reg).invalidate();
//    }
}

void ServiceRegistry::unregisterServices(osgi::Bundle* bundle)
{
//    // Simply remove all service registrations for the bundle.
//    ServiceRegistration[] regs = null;
//    synchronized (this)
//    {
//      regs = (ServiceRegistration[]) m_regsMap.get(bundle);
//    }

//    // Note, there is no race condition here with respect to the
//    // bundle registering more services, because its bundle context
//    // has already been invalidated by this point, so it would not
//    // be able to register more services.

//    // Unregister each service.
//    for (int i = 0; (regs != null) && (i < regs.length); i++)
//    {
//      if (((ServiceRegistrationImpl) regs[i]).isValid())
//      {
//        regs[i].unregister();
//      }
//    }

//    // Now remove the bundle itself.
//    synchronized (this)
//    {
//      m_regsMap.remove(bundle);
//    }
}

void ServiceRegistry::getUsingBundles(std::set<osgi::Bundle*>& result, const osgi::ServiceReferenceBase& ref)
{
//    Lock l(*this);
//    Bundle[] bundles = null;
//    for (Iterator iter = m_inUseMap.entrySet().iterator(); iter.hasNext(); )
//    {
//      Map.Entry entry = (Map.Entry) iter.next();
//      Bundle bundle = (Bundle) entry.getKey();
//      UsageCount[] usages = (UsageCount[]) entry.getValue();
//      for (int useIdx = 0; useIdx < usages.length; useIdx++)
//      {
//        if (usages[useIdx].m_ref.equals(ref))
//        {
//          // Add the bundle to the array to be returned.
//          if (bundles == null)
//          {
//            bundles = new Bundle[] { bundle };
//        }
//        else
//        {
//          Bundle[] nbs = new Bundle[bundles.length + 1];
//          System.arraycopy(bundles, 0, nbs, 0, bundles.length);
//          nbs[bundles.length] = bundle;
//          bundles = nbs;
//        }
//      }
//    }
//  }
//  return bundles;
}

ServiceRegistry::ServiceRegistry()
  : m_currentServiceId(1), m_bindingRegistry(new InterfaceBindingRegistry)
{
}

ServiceRegistry::~ServiceRegistry()
{
  delete m_bindingRegistry;
}
