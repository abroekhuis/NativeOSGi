
#include <dlfcn.h>
#include <stdio.h>

#include <test_driver_config.h>

int load_provider(const char* libPath)
{
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

  // Register service implementations
  register_func();
  return 0;
}

int load_consumer(const char* libPath)
{
  void* libHandle = dlopen(libPath, RTLD_LAZY);
  if (libHandle == 0)
  {
      printf("error: dlopen(%s) failed\n", libPath);
      return 1;
  }

  void* consume_symbol = dlsym(libHandle, "consume_service");
  if (consume_symbol == 0)
  {
      printf("error: symbol consume_service not found\n");
      return 2;
  }

  typedef void(*consume_func_t)();
  consume_func_t consume_func = (consume_func_t)consume_symbol;

  // Consume the C++ Greeting Service implementation via its C interface
  consume_func();
  return 0;
}

int main(int argc, char** argv)
{
  int use_cpp_provider = 0;
  if (argc > 1) use_cpp_provider = 1;

  if (use_cpp_provider)
  {
    // Register a service implementation provided by a C++ bundle
    int err = load_provider(ROOT_BINARY_DIR "cpp_greeting_provider/liborg_nativeosgi_cppgreetingserviceprovider.so");
    if (err) return err;
  }
  else
  {
    // Register a service implementation provided by a C++ bundle
    int err = load_provider(ROOT_BINARY_DIR "c_greeting_provider/liborg_nativeosgi_cgreetingserviceprovider.so");
    if (err) return err;
  }


  {
    // Register a complex service implemented in C++
    int err = load_provider(ROOT_BINARY_DIR "cpp_complexservice_provider/liborg_nativeosgi_cppcomplexserviceprovider.so");
    if (err) return err;
  }

  {
    // Register a service implemented in C++ where only a C++ interface is available
    int err = load_provider(ROOT_BINARY_DIR "cpp_cppserviceonly_provider/liborg_nativeosgi_cppserviceonlyprovider.so");
    if (err) return err;
  }

  {
    // Consume a C++ service via C bundle
    int err = load_consumer(ROOT_BINARY_DIR "c_consumer/liborg_nativeosgi_cserviceconsumer.so");
    if (err) return err;
  }

  {
    // Consume a C++ service via C++ bundle
    int err = load_consumer(ROOT_BINARY_DIR "cpp_consumer/liborg_nativeosgi_cppserviceconsumer.so");
    if (err) return err;
  }

  return 0;
}
