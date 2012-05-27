#ifndef SB_REGISTRY_C_API_H
#define SB_REGISTRY_C_API_H

#ifdef __cplusplus
extern "C"
#endif
void* CppRegistry_getService(const char* id);

#ifdef __cplusplus
extern "C"
#endif
void CppRegistry_registerService(const char* id, void* service);

#endif // SB_REGISTRY_C_API_H
