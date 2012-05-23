#include <IGreetingService.h>

#include <sb_registry.h>

#include <iostream>

extern "C" void consume_service()
{
  IGreetingService* greetingService = CppRegistry::instance()->getService<IGreetingService>();
  if (greetingService == 0)
  {
    std::cout << "IGreetingService not available" << std::endl;
    return;
  }

  std::cout << "C++ bundle consuming Greeting service" << std::endl;
  greetingService->sayHello();
}
