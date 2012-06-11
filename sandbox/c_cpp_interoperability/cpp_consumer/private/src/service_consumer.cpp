#include <IGreetingService.h>
#include <IComplexService.h>
#include <ICppOnlyService.h>

#include <ServiceRegistry.h>

#include <osgi/BundleContext.h>
#include <osgi/ServiceReference.h>

#include <iostream>

extern "C" void consume_service()
{
  osgi::BundleContext* context = ServiceRegistry::bundleContext();
  osgi::ServiceReference<IGreetingService> greetingRef = context->getServiceReference<IGreetingService>();
  if (!greetingRef)
  {
    std::cout << "IGreetingService not available" << std::endl;
  }
  else
  {
    IGreetingService* greetingService = context->getService(greetingRef);
    if (greetingService)
    {
      std::cout << "C++ bundle consuming Greeting service" << std::endl;
      greetingService->sayHello();
    }
    else
    {
      std::cout << "IGreetingService service pointer is NULL" << std::endl;
    }
  }

  osgi::ServiceReference<IComplexService> complexServiceRef = context->getServiceReference<IComplexService>();
  if (!complexServiceRef)
  {
    std::cout << "IComplexService not available" << std::endl;
  }
  else
  {
    IComplexService* complexService = context->getService(complexServiceRef);
    if (complexService)
    {
      std::cout << "C++ bundle consuming IComplexService service" << std::endl;
      std::vector<int> numbers;
      numbers.push_back(3);
      numbers.push_back(8);
      numbers.push_back(1);
      numbers.push_back(78);
      numbers.push_back(-4);
      numbers.push_back(25);
      std::cout << "Sorted numbers: " << complexService->ToSortedString(numbers) << std::endl;
    }
  }

  osgi::ServiceReference<ICppOnlyService> cppOnlyServiceRef = context->getServiceReference<ICppOnlyService>();
  if (!cppOnlyServiceRef)
  {
    std::cout << "ICppOnlyService not available" << std::endl;
  }
  else
  {
    ICppOnlyService* cppOnlyService = context->getService(cppOnlyServiceRef);
    if (cppOnlyService)
    {
      std::cout << "C++ bundle consuming ICppOnlyService service" << std::endl;
      cppOnlyService->doSomething();
    }
  }
}
