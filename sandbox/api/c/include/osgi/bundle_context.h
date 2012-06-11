
#ifndef BUNDLE_CONTEXT_H_
#define BUNDLE_CONTEXT_H_

#include <stdbool.h>

typedef struct bundleContext *BUNDLE_CONTEXT;
typedef struct serviceReference *SERVICE_REFERENCE;
typedef struct serviceRegistration *SERVICE_REGISTRATION;
typedef struct serviceProperties *PROPERTIES;

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C int bundleContext_create(BUNDLE_CONTEXT *bundle_context);

EXTERN_C int bundleContext_registerService(BUNDLE_CONTEXT context, char * serviceName, void * svcObj,
                                           PROPERTIES properties, SERVICE_REGISTRATION *service_registration);

EXTERN_C int bundleContext_getServiceReference(BUNDLE_CONTEXT context, char * serviceName,
                                               SERVICE_REFERENCE *service_reference);

EXTERN_C int bundleContext_getService(BUNDLE_CONTEXT context, SERVICE_REFERENCE reference, void **service_instance);
EXTERN_C int bundleContext_ungetService(BUNDLE_CONTEXT context, SERVICE_REFERENCE reference, bool *result);

#endif /* BUNDLE_CONTEXT_H_ */
