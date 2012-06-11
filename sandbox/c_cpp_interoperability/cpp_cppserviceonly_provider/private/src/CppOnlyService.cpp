
#include <ICppOnlyService.h>

#include <ServiceRegistry.h>
#include <osgi/BundleContext.h>

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
  ServiceRegistry::bundleContext()->registerService<ICppOnlyService>(new CppOnlyService);
}
