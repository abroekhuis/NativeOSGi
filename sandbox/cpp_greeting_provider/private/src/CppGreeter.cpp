
#include <IGreetingService.h>

#include <sb_registry.h>

#include <iostream>

class CppGreeter : public IGreetingService
{
public:

  void sayHello() const
  {
    std::cout << "Hello from a C++ Greeting service" << std::endl;
  }
};

extern "C" void register_services()
{
  CppRegistry::instance()->registerService<IGreetingService>(new CppGreeter);
}
