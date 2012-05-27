
#include "IComplexService.h"

#include <stdlib.h>

#include <iostream>

IComplexService::~IComplexService()
{}

class ComplexServiceCWrapper : public IComplexService
{
private:

  complex_service_t c_impl;

public:

  ComplexServiceCWrapper(complex_service_t c_impl_)
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

#define JOIN(x, y) JOIN_AGAIN(x, y)
#define JOIN_AGAIN(x, y) x ## y

#define C_TO_CPP_FUNCTION(c_struct, cpp_interface) \
  extern "C" cpp_interface* \
  JOIN(cpp_interface ## _ID, _ctocpp) (void* c_struct)

C_TO_CPP_FUNCTION(c_struct, IComplexService)
{
  // need to manage the memory for the wrapper.
  // probably cache instances.
  return new ComplexServiceCWrapper((complex_service_t)c_struct);
}
