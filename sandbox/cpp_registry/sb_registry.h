#ifndef sb_registry_h__
#define sb_registry_h__

#include "sb_osgi_interface.h"

#include <string>
#include <map>
#include <iostream>

#include <typeinfo>
#include <dlfcn.h>

#include "sb_registry_c_api.h"


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
class CppRegistry
{

  friend void* CppRegistry_getService(const char*);

  struct ServiceHandle {
    bool isCppService;
    bool hasCInterface;
    void* handle;
  };


public:

  static CppRegistry* instance();

  void registerService(const std::string& interfaceId, void* impl, bool isCpp, bool isC);

  template<class S>
  void registerService(S* impl)
  {
    const char* interfaceId = osgi_interface_id<S*>();
    if (interfaceId == NULL)
    {
      std::cerr << "error: Missing OSGI_DECLARE_SERVICE_INTERFACE macro for " << typeid(S).name() << std::endl;
      return;
    }

    // We are registering a C++ service implementation, using the C++ service interface

    /*
     * Check if the interface bundle (or any other shared library which is in the symbol
     * look-up path) provides a C function which creates a corresponding C interface struct which
     * maps individual C function calls to the C++ service interface methods.
     */
    std::string cpptoc_symbolname = std::string(interfaceId) + "_cpptoc";
    void* cpptoc_symbol = dlsym(RTLD_DEFAULT, cpptoc_symbolname.c_str());
    if (cpptoc_symbol == NULL)
    {
      // No interface specific C to C++ mapping available. We just create a generic
      // C struct which wraps the pointer to the C++ service class instance.

      std::cout << "warning: " << cpptoc_symbolname << " symbol not found. C++ service will not be available via C." << std::endl;
      struct instance_handle {
        void* instance;
      };
      instance_handle* instance_h = new instance_handle;
      instance_handle* cppinstance_h = new instance_handle;
      cppinstance_h->instance = static_cast<void*>(impl);
      instance_h->instance = cppinstance_h;
      // This is a C++ service implementation, having no C interface.
      registerService(interfaceId, instance_h, true, false);
    }
    else
    {
      typedef void* (*cpptoc_handle)(void*);
      cpptoc_handle cpptoc = reinterpret_cast<cpptoc_handle>(cpptoc_symbol);
      void* c_handle = cpptoc(static_cast<void*>(impl));
      // This is a C++ service implementation, having a C interface
      registerService(interfaceId, c_handle, true, true);
    }
  }

  template<class S>
  S* getService() const
  {
    const char* interfaceId = osgi_interface_id<S*>();
    if (interfaceId == NULL)
    {
      std::cerr << "error: Missing OSGI_DECLARE_SERVICE_INTERFACE macro for " << typeid(S).name() << std::endl;
      return 0;
    }

    ServiceHandle serviceHandle = getService(interfaceId);
    S* service = 0;
    if (serviceHandle.handle)
    {
      if (serviceHandle.isCppService)
      {
        /*
         * Short-cut. Consuming a C++ service implementation through its C++ interface
         * should not go through the C interface (there might not even be one). More
         * importantly, this avoids argument conversions to C and back to C++ types.
         */
        struct instance_handle {
          void* instance;
        };
        instance_handle* instance_h = reinterpret_cast<instance_handle*>(serviceHandle.handle);
        instance_handle* cppinstance_h = reinterpret_cast<instance_handle*>(instance_h->instance);
        service = reinterpret_cast<S*>(cppinstance_h->instance);
      }
      else
      {
        /*
         * Get the interface specific C++ wrapper for the C struct. We look for a special
         * symbol, which is usually implemented in the interface bundle but could also
         * be found in any shared library which is in the symbol look-up path of the
         * calling library (service consumer).
         */

        std::string ctocpp_symbolname = std::string(interfaceId) + "_ctocpp";
        void* ctocpp_symbol = dlsym(RTLD_DEFAULT, ctocpp_symbolname.c_str());
        if (ctocpp_symbol == NULL)
        {
          // There is no C++ interface wrapper for the C service implementation available
          std::cerr << "error: " << ctocpp_symbolname << " symbol not found" << std::endl;
          return NULL;
        }

        typedef void* (*ctocpp_handle)(void*);
        ctocpp_handle ctocpp = reinterpret_cast<ctocpp_handle>(ctocpp_symbol);

        /**
         * A call to this function returns a custom C++ interface implementation
         * mapping the C++ interface method calls to the C functions provided
         * by this C service implementation.
         */
        service = reinterpret_cast<S*>(ctocpp(serviceHandle.handle));
      }
    }
    return service;
  }

  ServiceHandle getService(const std::string& interfaceId) const;

private:

  std::map<std::string, ServiceHandle> m_MapIdToServiceHandle;
};

#endif
