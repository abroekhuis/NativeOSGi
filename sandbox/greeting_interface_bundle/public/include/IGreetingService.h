#include "greeting_service.h"

#include <sb_osgi_interface.h>

struct IGreetingService {
  // anchor the vtable
  virtual ~IGreetingService();

  virtual void sayHello() const = 0;
};

OSGI_DECLARE_SERVICE_INTERFACE(IGreetingService)
