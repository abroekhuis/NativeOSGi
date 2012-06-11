
#include <osgi/bundle_context.h>

#include <greeting_service.h>
#include <complex_service.h>

#include <stdio.h>
#include <stdlib.h>

void consume_service()
{
  BUNDLE_CONTEXT bundleContext = NULL;
  if (bundleContext_create(&bundleContext))
  {
    fprintf(stderr, "Creating a bundle context failed.\n");
    return;
  }

  SERVICE_REFERENCE serviceRef = NULL;
  if (bundleContext_getServiceReference(bundleContext, IGreetingService_NAME, &serviceRef))
  {
    fprintf(stderr, "Getting service reference failed.\n");
    return;
  }

  if (serviceRef)
  {
    void* serviceHandle = NULL;
    if (bundleContext_getService(bundleContext, serviceRef, &serviceHandle))
    {
      fprintf(stderr, "Getting service failed.\n");
      return;
    }
    if (serviceHandle)
    {
      printf("C bundle consuming greeting service\n");

      greeting_service_t service = (greeting_service_t)serviceHandle;
      service->greeting_sayHello(service->instance);
    }
    else
    {
      printf("Service pointer for " IGreetingService_NAME " is NULL\n");
    }
  }
  else
  {
    printf("Service " IGreetingService_NAME " not available.\n");
  }

  // --------------- Consume complex C++ service ---------------------------

  serviceRef = NULL;
  if (bundleContext_getServiceReference(bundleContext, IComplexService_NAME, &serviceRef))
  {
    fprintf(stderr, "Getting service reference failed.\n");
    return;
  }

  if (serviceRef)
  {
    void* serviceHandle = NULL;
    if (bundleContext_getService(bundleContext, serviceRef, &serviceHandle))
    {
      fprintf(stderr, "Getting service failed.\n");
      return;
    }
    if (serviceHandle)
    {
      printf("C bundle consuming complex C++ service\n");

      complex_service_t complex_service = (complex_service_t)serviceHandle;
      vector_const vec_numbers;
      int numbers[6] = { 3, 8, 1, 78, -4, 25 };
      vec_numbers.size = 6;
      vec_numbers.data = &numbers;
      char* sorted = complex_service->complex_service_toSortedString(complex_service->instance, &vec_numbers);
      printf("Sorted numbers: %s\n", sorted);
      free(sorted);
    }
    else
    {
      printf("Service pointer for " IComplexService_NAME " is NULL\n");
    }
  }
  else
  {
    printf("Service " IComplexService_NAME " not available.\n");
  }

  // ------------------ Try to consume a C++ only service ---------------------------

  printf("C bundle trying to consume C++ only interface\n");

  // see what happens if we try to access a service which is only available via its C++ interface
  serviceRef = NULL;
  if (bundleContext_getServiceReference(bundleContext, "org_nativeosgi_ICppOnlyService_1_0", &serviceRef))
  {
    printf("Getting service reference for org_nativeosgi_ICppOnlyService_1_0 failed\n");
    return;
  }

  if (serviceRef)
  {
    printf("org_nativeosgi_ICppOnlyService_1_0 found but should not be available\n");
  }
  else
  {
    printf("Service reference for org_nativeosgi_ICppOnlyService_1_0 is NULL, as it should be.\n");
  }
}
