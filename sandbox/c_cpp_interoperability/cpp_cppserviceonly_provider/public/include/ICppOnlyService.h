#ifndef ICppOnlyService_h__
#define ICppOnlyService_h__

#include <osgi/ServiceInterface.h>

#define ICppOnlyService_ID org_nativeosgi_ICppOnlyService_1_0
#define ICppOnlyService_NAME "org_nativeosgi_ICppOnlyService_1_0"

struct ICppOnlyService
{
  // anchor the vtable
  virtual ~ICppOnlyService();

  virtual void doSomething() = 0;
};

OSGI_DECLARE_SERVICE_INTERFACE(ICppOnlyService)

#endif // ICppOnlyService_h__
