
#include <IGreetingService.h>

#include <ServiceRegistry.h>
#include <osgi/BundleContext.h>

#include <iostream>

class CppGreeter : public IGreetingService
{
public:

  void sayHello() const
  {
    std::cout << "Greetings from a C++ Greeting service (" << osgi_service_interface_id<IGreetingService*>() << ")" << std::endl;
  }
};

extern "C" void register_services()
{
  std::cout << "C++ bundle registering Greeting service" << std::endl;
  ServiceRegistry::bundleContext()->registerService<IGreetingService>(new CppGreeter);
}
