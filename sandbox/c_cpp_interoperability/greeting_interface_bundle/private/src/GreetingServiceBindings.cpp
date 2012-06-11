
#include "greeting_service.h"
#include "IGreetingService.h"

#include <osgi/BindingMacros.h>

#include <iostream>

IGreetingService::~IGreetingService()
{}


// ----------------------- C bindings ------------------------

struct greeting {
  void* cppHandle;
};

extern "C" void greeting_sayHello(greeting_t instance)
{
  IGreetingService* greetingService =
      reinterpret_cast<IGreetingService*>(instance->cppHandle);
  std::cout << "Calling C++ GreetingService via C interface" << std::endl;
  greetingService->sayHello();
}


OSGI_CREATE_C_BINDING(IGreetingService_ID, cpp_instance)
{
  greeting_service_t c_impl = new greeting_service;
  greeting_t instance = new greeting;
  instance->cppHandle = cpp_instance;
  c_impl->instance = instance;
  c_impl->greeting_sayHello = greeting_sayHello;
  return c_impl;
}

OSGI_DESTROY_C_BINDING(IGreetingService_ID, c_struct)
{
  delete reinterpret_cast<greeting_service_t>(c_struct);
}


// -------------------- C++ bindings ----------------------


class GreetingServiceCppBinding : public IGreetingService
{
private:

  greeting_service_t c_impl;

public:

  GreetingServiceCppBinding(greeting_service_t c_impl_)
   : c_impl(c_impl_)
  {}

  void sayHello() const
  {
    std::cout << "Calling C GreetingService via C++ interface" << std::endl;
    c_impl->greeting_sayHello(c_impl->instance);
  }
};


OSGI_CREATE_CPP_BINDING(IGreetingService_ID, greeting_service)
{
  return static_cast<IGreetingService*>(new GreetingServiceCppBinding((greeting_service_t)greeting_service));
}

OSGI_DESTROY_CPP_BINDING(IGreetingService_ID, cpp_wrapper)
{
  delete reinterpret_cast<IGreetingService*>(cpp_wrapper);
}

OSGI_REGISTER_BINDINGS_BEGIN
  OSGI_REGISTER_C_BINDING(IGreetingService_ID)
  OSGI_REGISTER_CPP_BINDING(IGreetingService_ID)
OSGI_REGISTER_BINDINGS_END
