#include <sb_registry_c_api.h>

#include <greeting_service.h>
#include <complex_service.h>

#include <stdio.h>
#include <stdlib.h>

void consume_service()
{
  void* serviceHandle = CppRegistry_getService(IGreetingService_NAME);
  if (serviceHandle == 0)
  {
    printf(IGreetingService_NAME " not available\n");
    exit(3);
  }

  printf("C bundle consuming greeting service\n");

  greeting_service_t service = (greeting_service_t)serviceHandle;
  service->greeting_sayHello(service->instance);

  serviceHandle = CppRegistry_getService(IComplexService_NAME);
  if (serviceHandle == 0)
  {
    printf(IComplexService_NAME " not available\n");
    exit(3);
  }

  printf("C bundle consuming complex service\n");

  complex_service_t complex_service = (complex_service_t)serviceHandle;
  vector_const vec_numbers;
  int numbers[6] = { 3, 8, 1, 78, -4, 25 };
  vec_numbers.size = 6;
  vec_numbers.data = &numbers;
  char* sorted = complex_service->complex_service_toSortedString(complex_service->instance, &vec_numbers);
  printf("Sorted numbers: %s\n", sorted);
  free(sorted);

  printf("C bundle trying to consume c++ only interface\n");

  // see what happens if we try to access a service which is only available via its C++ interface
  serviceHandle = CppRegistry_getService("org_nativeosgi_ICppOnlyService_1_0");
  if (serviceHandle == 0)
  {
    printf("org_nativeosgi_ICppOnlyService_1_0 not available via C\n");
  }
  else
  {
    printf("org_nativeosgi_ICppOnlyService_1_0 found but should not be available");
  }
}
