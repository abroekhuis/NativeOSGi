#include <sb_registry_c_api.h>

#include <greeting_service.h>

#include <stdio.h>
#include <stdlib.h>

void consume_service()
{
    void* serviceHandle = CppRegistry_getService(IGreetingService_NAME);
    if (serviceHandle == 0)
    {
        printf("IGreetingService not available\n");
        exit(3);
    }

    printf("C bundle consuming greeting service\n");

    greeting_service_t service = (greeting_service_t)serviceHandle;
    service->greeting_sayHello(service->instance);
}
