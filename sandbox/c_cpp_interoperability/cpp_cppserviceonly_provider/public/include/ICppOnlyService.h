#ifndef ICppOnlyService_h__
#define ICppOnlyService_h__

#include <osgi/ServiceInterface.h>

#define ICppOnlyService_ID org_nativeosgi_ICppOnlyService_1_0
#define ICppOnlyService_NAME OSGI_XSTR(ICppOnlyService_ID)

struct ICppOnlyService
{
  // anchor the vtable
  virtual ~ICppOnlyService();

  virtual void doSomething() = 0;
};

OSGI_DECLARE_SERVICE_INTERFACE(ICppOnlyService, ICppOnlyService_NAME)

#endif // ICppOnlyService_h__
