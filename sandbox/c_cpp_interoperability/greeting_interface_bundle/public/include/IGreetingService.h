#ifndef IGreetingService_h__
#define IGreetingService_h__

#include "greeting_service.h"

#include <osgi/ServiceInterface.h>

struct IGreetingService {
  // anchor the vtable
  virtual ~IGreetingService();

  virtual void sayHello() const = 0;
};

OSGI_DECLARE_SERVICE_INTERFACE(IGreetingService, IGreetingService_NAME)

#endif
