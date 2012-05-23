
#include "greeting_service.h"
#include "IGreetingService.h"

struct greeting {
  void* cppHandle;
};

extern "C" void greeting_sayHello(greeting_t instance) {
  IGreetingService* greetingService =
    reinterpret_cast<IGreetingService*>(instance->cppHandle);
  greetingService->sayHello();
}

#define JOIN(x, y) JOIN_AGAIN(x, y)
#define JOIN_AGAIN(x, y) x ## y

#define CPP_TO_C_FUNCTION(cpp_interface) \
  extern "C" void* /* greeting_service_t */ \
  JOIN(IGreetingService_ID, _cpptoc) (void* /* IGreetingService* */ cpp_interface)


CPP_TO_C_FUNCTION(impl)
{
  greeting_service_t c_impl = new greeting_service; 
  greeting_t instance = new greeting;
  instance->cppHandle = impl;
  c_impl->instance = instance;
  c_impl->greeting_sayHello = greeting_sayHello;
  return c_impl;
}
