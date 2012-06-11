
#pragma once
#ifndef OSGI_BINDINGMACROS_H_
#define OSGI_BINDINGMACROS_H_

#define OSGI_JOIN(x, y) OSGI_JOIN_AGAIN(x, y)
#define OSGI_JOIN_AGAIN(x, y) x ## y

#define OSGI_XSTR(x) OSGI_STR(x)
#define OSGI_STR(x) #x

typedef void*(*osgi_create_c_binding)(void*);
typedef void*(*osgi_create_cpp_binding)(void*);

typedef void(*osgi_destroy_c_binding)(void*);
typedef void(*osgi_destroy_cpp_binding)(void*);

typedef void(*osgi_register_c_binding)(void*, const char*, osgi_create_c_binding, osgi_destroy_c_binding);
typedef void(*osgi_register_cpp_binding)(void*, const char*, osgi_create_cpp_binding, osgi_destroy_cpp_binding);

typedef struct
{
  void* instance;
  osgi_register_c_binding register_c_binding;
  osgi_register_cpp_binding register_cpp_binding;
} osgi_binding_registry;

#define OSGI_CREATE_C_BINDING(interface_id, cpp_interface) \
  extern "C" void* /* some_service_t */ \
  OSGI_JOIN(interface_id, _create_c_binding) (void* /* ISomeService* */ cpp_interface)

#define OSGI_DESTROY_C_BINDING(interface_id, c_struct) \
  extern "C" void \
  OSGI_JOIN(interface_id, _destroy_c_binding) (void* /* some_service_t */ c_struct)

#define OSGI_CREATE_CPP_BINDING(interface_id, c_struct) \
  extern "C" void* \
  OSGI_JOIN(interface_id, _create_cpp_binding) (void* /* some_service_t */ c_struct)

#define OSGI_DESTROY_CPP_BINDING(interface_id, cpp_interface) \
  extern "C" void \
  OSGI_JOIN(interface_id, _destroy_cpp_binding) (void* /* ISomeService */ cpp_interface)

#define OSGI_REGISTER_BINDINGS_BEGIN extern "C" void osgi_register_bindings(osgi_binding_registry* registry) {
#define OSGI_REGISTER_C_BINDING(interface_id) \
  registry->register_c_binding(registry->instance, OSGI_XSTR(interface_id), OSGI_JOIN(interface_id, _create_c_binding), OSGI_JOIN(interface_id, _destroy_c_binding));
#define OSGI_REGISTER_CPP_BINDING(interface_id) \
  registry->register_cpp_binding(registry->instance, OSGI_XSTR(interface_id), OSGI_JOIN(interface_id, _create_cpp_binding), OSGI_JOIN(interface_id, _destroy_cpp_binding));
#define OSGI_REGISTER_BINDINGS_END }

#endif // OSGI_BINDINGMACROS_H_
