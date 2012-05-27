#include "sb_registry.h"


CppRegistry* CppRegistry::instance()
{
  static CppRegistry registry;
  return &registry;
}

void CppRegistry::registerService(const std::string& interfaceId, void* impl, bool isCpp, bool isC)
{
  std::cout << "Registering service " << interfaceId << " (addr " << impl << ")" << std::endl;
  ServiceHandle serviceHandle = { isCpp, isC, impl };
  m_MapIdToServiceHandle[interfaceId] = serviceHandle;
}

CppRegistry::ServiceHandle CppRegistry::getService(const std::string &interfaceId) const
{
    std::map<std::string,ServiceHandle>::const_iterator i = m_MapIdToServiceHandle.find(interfaceId);
    if (i != m_MapIdToServiceHandle.end())
      return i->second;
    ServiceHandle none = {false, false, 0};
    return none;
}


//---------------------------------------------------
// C interface
//---------------------------------------------------

void* CppRegistry_getService(const char* id)
{
  CppRegistry::ServiceHandle serviceHandle = CppRegistry::instance()->getService(id);
  if (serviceHandle.hasCInterface)
  {
    return serviceHandle.handle;
  }
  return 0;
}

void CppRegistry_registerService(const char* id, void* service)
{
  CppRegistry::instance()->registerService(id, service, false, true);
}
