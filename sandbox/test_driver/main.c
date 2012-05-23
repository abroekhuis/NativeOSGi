
#include <dlfcn.h>
#include <stdio.h>

#include <test_driver_config.h>

int main(int argc, char** argv)
{
  // Register a service implementation
  {
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

  // Consume a service
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

    // Register the C++ Greeting Service implementation
    consume_func();
    return 0;
  }
}
