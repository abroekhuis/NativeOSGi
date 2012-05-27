
#include "IComplexService.h"

#include <string.h>
#include <stdlib.h>

#include <iostream>

struct complex_data {
  void* cppHandle;
};

extern "C" char* complex_service_toSortedString(complex_data_t instance, vector_const_t int_vector)
{
  IComplexService* complexService =
    reinterpret_cast<IComplexService*>(instance->cppHandle);
  std::vector<int> numbers(reinterpret_cast<const int*>(int_vector->data), reinterpret_cast<const int*>(int_vector->data) + int_vector->size);
  std::cout << "Calling C++ interface methods from C function" << std::endl;
  std::string str = complexService->ToSortedString(numbers);
  char* c_str = (char*)malloc(sizeof(char)*(str.size()+1));
  strcpy(c_str, str.c_str());
  return c_str;
}

#define JOIN(x, y) JOIN_AGAIN(x, y)
#define JOIN_AGAIN(x, y) x ## y

#define CPP_TO_C_FUNCTION(cpp_interface) \
  extern "C" void* /* greeting_service_t */ \
  JOIN(IComplexService_ID, _cpptoc) (void* /* IGreetingService* */ cpp_interface)


CPP_TO_C_FUNCTION(impl)
{
  complex_data_t instance = new complex_data;
  instance->cppHandle = impl;

  complex_service_t c_impl = new complex_service;
  c_impl->instance = instance;
  c_impl->complex_service_toSortedString = complex_service_toSortedString;
  return c_impl;
}
