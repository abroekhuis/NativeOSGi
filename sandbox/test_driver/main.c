
#include <dlfcn.h>
#include <stdio.h>

#include <test_driver_config.h>

int main(int argc, char** argv)
{
  int use_cpp_provider = 0;
  if (argc > 1) use_cpp_provider = 1;

  if (use_cpp_provider)
  {
    // Register a service implementation provided by a C++ bundle

    const char* libPath = ROOT_BINARY_DIR "cpp_greeting_provider/liborg_nativeosgi_cppgreetingserviceprovider.so";
    void* libHandle = dlopen(libPath, RTLD_LAZY);
    if (libHandle == 0)
    {
        printf("error: dlopen(%s) failed\n", libPath);
        return 1;
    }

    void* register_symbol = dlsym(libHandle, "register_services");
    if (register_symbol == 0)
    {
        printf("error: symbol register_services not found\n");
        return 2;
    }

    typedef void(*register_func_t)();
    register_func_t register_func = (register_func_t)register_symbol;

    // Register the C++ Greeting Service implementation
    register_func();
  }
  else
  {
    // Register a service implementation provided by a C++ bundle

    const char* libPath = ROOT_BINARY_DIR "c_greeting_provider/liborg_nativeosgi_cgreetingserviceprovider.so";
    void* libHandle = dlopen(libPath, RTLD_LAZY);
    if (libHandle == 0)
    {
        printf("error: dlopen(%s) failed\n", libPath);
        return 1;
    }

    void* register_symbol = dlsym(libHandle, "register_services");
    if (register_symbol == 0)
    {
        printf("error: symbol register_services not found\n");
        return 2;
    }

    typedef void(*register_func_t)();
    register_func_t register_func = (register_func_t)register_symbol;

    // Register the C++ Greeting Service implementation
    register_func();
  }

  // Consume a C++ service via C bundle
  {
    const char* libPath = ROOT_BINARY_DIR "c_greeting_consumer/liborg_nativeosgi_cgreetingserviceconsumer.so";
    void* libHandle = dlopen(libPath, RTLD_LAZY);
    if (libHandle == 0)
    {
        printf("error: dlopen(%s) failed\n", libPath);
        return 1;
    }

    void* register_symbol = dlsym(libHandle, "consume_service");
    if (register_symbol == 0)
    {
        printf("error: symbol consume_service not found\n");
        return 2;
    }

    typedef void(*consume_func_t)();
    consume_func_t consume_func = (consume_func_t)register_symbol;

    // Consume the C++ Greeting Service implementation via its C interface
    consume_func();
  }

  // Consume a C++ service via C++ bundle
  {
    const char* libPath = ROOT_BINARY_DIR "cpp_greeting_consumer/liborg_nativeosgi_cppgreetingserviceconsumer.so";
    void* libHandle = dlopen(libPath, RTLD_LAZY);
    if (libHandle == 0)
    {
        printf("error: dlopen(%s) failed\n", libPath);
        return 1;
    }

    void* register_symbol = dlsym(libHandle, "consume_service");
    if (register_symbol == 0)
    {
        printf("error: symbol consume_service not found\n");
        return 2;
    }

    typedef void(*consume_func_t)();
    consume_func_t consume_func = (consume_func_t)register_symbol;

    // Consume the C++ Greeting Service implementation via its C++ interface
    consume_func();
  }

  return 0;
}
