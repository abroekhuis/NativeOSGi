
#include <ICppOnlyService.h>

#include <sb_registry.h>

#include <iostream>

class CppOnlyService : public ICppOnlyService
{

public:

  void doSomething()
  {
    std::cout << "Cpp Only Service doing something" << std::endl;
  }
};

extern "C" void register_services()
{
  std::cout << "C++ bundle registering C++ only service" << std::endl;
  CppRegistry::instance()->registerService<ICppOnlyService>(new CppOnlyService);
}
