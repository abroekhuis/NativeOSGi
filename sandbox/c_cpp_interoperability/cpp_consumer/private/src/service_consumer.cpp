#include <IGreetingService.h>
#include <IComplexService.h>
#include <ICppOnlyService.h>

#include <sb_registry.h>

#include <iostream>

extern "C" void consume_service()
{
  IGreetingService* greetingService = CppRegistry::instance()->getService<IGreetingService>();
  if (greetingService == 0)
  {
    std::cout << "IGreetingService not available" << std::endl;
  }
  else
  {
    std::cout << "C++ bundle consuming Greeting service" << std::endl;
    greetingService->sayHello();
  }

  IComplexService* complexService = CppRegistry::instance()->getService<IComplexService>();
  if (complexService == 0)
  {
    std::cout << "IComplexService not available" << std::endl;
  }
  else
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

  ICppOnlyService* cppOnlyService = CppRegistry::instance()->getService<ICppOnlyService>();
  if (cppOnlyService == 0)
  {
    std::cout << "ICppOnlyService not available" << std::endl;
  }
  else
  {
    std::cout << "C++ bundle consuming ICppOnlyService service" << std::endl;
    cppOnlyService->doSomething();
  }
}
