
#include <osgi/bundle_context.h>

#include <osgi/BundleContext.h>

#include <ServiceRegistry.h>

struct bundleContext {
  osgi::BundleContext* handle;
};

struct serviceReference {
  osgi::ServiceReferenceBase handle;
};

int bundleContext_create(BUNDLE_CONTEXT *bundle_context)
{
  bundleContext* context = new bundleContext;
  context->handle = ServiceRegistry::bundleContext();
  *bundle_context = context;
  return 0;
}

int bundleContext_registerService(BUNDLE_CONTEXT bundle_context, char * serviceName, void * svcObj,
                                  PROPERTIES properties, SERVICE_REGISTRATION *service_registration)
{
  osgi::BundleContext* context = bundle_context->handle;
  std::map<std::string,void*> svcPointers;
  svcPointers.insert(std::make_pair(std::string(serviceName), svcObj));
  context->registerService(svcPointers, false, osgi::ServiceProperties());
  return 0;
}

int bundleContext_getServiceReference(BUNDLE_CONTEXT bundle_context, char * serviceName, SERVICE_REFERENCE *service_reference)
{
  osgi::BundleContext* context = bundle_context->handle;
  osgi::ServiceReferenceBase serviceRef = context->getServiceReference(serviceName, false);
  if (!serviceRef)
  {
    *service_reference = NULL;
    return 0;
  }

  serviceReference* ref = new serviceReference;
  ref->handle = serviceRef;
  *service_reference = ref;
  return 0;
}

int bundleContext_getService(BUNDLE_CONTEXT bundle_context, SERVICE_REFERENCE reference, void **service_instance)
{
  osgi::BundleContext* context = bundle_context->handle;
  osgi::ServiceReferenceBase ref = reference->handle;

  *service_instance = context->getService(ref);

  return 0;
}

int bundleContext_ungetService(BUNDLE_CONTEXT context, SERVICE_REFERENCE reference, bool *result)
{
  return 0;
}

