#ifndef sb_registry_h__
#define sb_registry_h__

#include "sb_osgi_interface.h"

#include <string>
#include <map>
#include <iostream>

#include <typeinfo>
#include <dlfcn.h>

class CppRegistry
{
public:

  static CppRegistry* instance();

  void registerService(const std::string& interfaceId, void* impl);

  template<class S>
  void registerService(S* impl)
  {
    const char* interfaceId = osgi_interface_id<S*>();
    if (interfaceId == NULL)
    {
      std::cerr << "error: Missing OSGI_DECLARE_SERVICE_INTERFACE macro for " << typeid(S).name() << std::endl;
      return;
    }
    std::string cpptoc_symbolname = std::string(interfaceId) + "_cpptoc";
    void* cpptoc_symbol = dlsym(RTLD_DEFAULT, cpptoc_symbolname.c_str());
    if (cpptoc_symbol == NULL)
    {
      std::cerr << "error: " << cpptoc_symbolname << " symbol not found" << std::endl;
      return;
    }
    typedef void* (*cpptoc_handle)(void*);
    cpptoc_handle cpptoc = reinterpret_cast<cpptoc_handle>(cpptoc_symbol);
    void* c_handle = cpptoc(static_cast<void*>(impl));
    registerService(interfaceId, c_handle);
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
    std::string ctocpp_symbolname = std::string(interfaceId) + "_ctocpp";
    void* ctocpp_symbol = dlsym(RTLD_DEFAULT, ctocpp_symbolname.c_str());
    if (ctocpp_symbol == NULL)
    {
      std::cerr << "error: " << ctocpp_symbolname << " symbol not found" << std::endl;
      return NULL;
    }

    typedef void* (*ctocpp_handle)(void*);
    ctocpp_handle ctocpp = reinterpret_cast<ctocpp_handle>(ctocpp_symbol);

    void* serviceHandle = getService(interfaceId);
    S* service = 0;
    if (serviceHandle)
    {
      service = reinterpret_cast<S*>(ctocpp(serviceHandle));
    }
    return service;
  }

  void* getService(const std::string& interfaceId) const;

private:

  std::map<std::string, void*> m_MapIdToService;
};

#endif
