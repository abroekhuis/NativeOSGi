#include "sb_registry.h"

#include "sb_registry_c_api.h"

CppRegistry* CppRegistry::instance()
{
    static CppRegistry registry;
    return &registry;
}

void CppRegistry::registerService(const std::string& interfaceId, void* impl)
{
    std::cout << "Registering service " << interfaceId << " (addr " << impl << ")" << std::endl;
    m_MapIdToService[interfaceId] = impl;
}

void* CppRegistry::getService(const std::string &interfaceId) const
{
    std::map<std::string,void*>::const_iterator i = m_MapIdToService.find(interfaceId);
    if (i != m_MapIdToService.end())
      return i->second;
    return 0;
}


void* CppRegistry_getService(const char* id)
{
    return CppRegistry::instance()->getService(id);
}
