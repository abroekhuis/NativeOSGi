#ifndef IComplexService_h__
#define IComplexService_h__

#include "complex_service.h"

#include <osgi/ServiceInterface.h>

#include <vector>
#include <string>

struct IComplexService
{
  // anchor the vtable
  virtual ~IComplexService();

  virtual std::string ToSortedString(const std::vector<int>& ) const = 0;
};

OSGI_DECLARE_SERVICE_INTERFACE(IComplexService)

#endif // IComplexService_h__
