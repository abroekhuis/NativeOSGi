
#include "IComplexService.h"

#include <osgi/BindingMacros.h>

#include <string.h>
#include <stdlib.h>

#include <iostream>

IComplexService::~IComplexService()
{}

// -------------------------- C++ bindings --------------------------

class ComplexServiceCppBinding : public IComplexService
{
private:

  complex_service_t c_impl;

public:

  ComplexServiceCppBinding(complex_service_t c_impl_)
   : c_impl(c_impl_)
  {}

  std::string ToSortedString(const std::vector<int>& numbers) const
  {
    if (numbers.empty()) return "";

    vector_const tmp;
    tmp.size = numbers.size();
    tmp.data = &numbers[0];

    std::cout << "Calling C function from C++ interface" << std::endl;
    char* c_str = c_impl->complex_service_toSortedString(c_impl->instance, &tmp);
    std::string str(c_str);
    free(c_str);
    return str;
  }
};

OSGI_CREATE_CPP_BINDING(IComplexService_ID, c_struct)
{
  return static_cast<IComplexService*>(new ComplexServiceCppBinding((complex_service_t)c_struct));
}

OSGI_DESTROY_CPP_BINDING(IComplexService_ID, cpp_interface)
{
  delete reinterpret_cast<IComplexService*>(cpp_interface);
}

// ------------------------ C bindings ---------------------

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


OSGI_CREATE_C_BINDING(IComplexService_ID, impl)
{
  complex_data_t instance = new complex_data;
  instance->cppHandle = impl;

  complex_service_t c_impl = new complex_service;
  c_impl->instance = instance;
  c_impl->complex_service_toSortedString = complex_service_toSortedString;
  return c_impl;
}

OSGI_DESTROY_C_BINDING(IComplexService_ID, c_struct)
{
  delete static_cast<complex_service_t>(c_struct);
}

OSGI_REGISTER_BINDINGS_BEGIN
  OSGI_REGISTER_C_BINDING(IComplexService_ID)
  OSGI_REGISTER_CPP_BINDING(IComplexService_ID)
OSGI_REGISTER_BINDINGS_END
