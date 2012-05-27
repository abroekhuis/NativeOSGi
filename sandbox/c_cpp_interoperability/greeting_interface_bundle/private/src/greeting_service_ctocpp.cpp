
#include "greeting_service.h"
#include "IGreetingService.h"

IGreetingService::~IGreetingService()
{}

class GreetingServiceCWrapper : public IGreetingService
{
private:

  greeting_service_t c_impl;

public:

  GreetingServiceCWrapper(greeting_service_t c_impl_)
   : c_impl(c_impl_)
  {}

  void sayHello() const
  {
    c_impl->greeting_sayHello(c_impl->instance);
  }
};

#define JOIN(x, y) JOIN_AGAIN(x, y)
#define JOIN_AGAIN(x, y) x ## y

#define C_TO_CPP_FUNCTION(c_struct) \
  extern "C" IGreetingService* \
  JOIN(IGreetingService_ID, _ctocpp) (void* /* IGreetingService* */ c_struct)

C_TO_CPP_FUNCTION(greeting_service)
{
  // need to manage the memory for the wrapper.
  // probably cache instances.
  return new GreetingServiceCWrapper((greeting_service_t)greeting_service);
}
