
#include <IComplexService.h>

#include <sb_registry.h>

#include <iostream>
#include <sstream>
#include <algorithm>

class ComplexService : public IComplexService
{

public:

  std::string ToSortedString(const std::vector<int>& numbers) const
  {
    std::vector<int> tmp(numbers);
    std::sort(tmp.begin(), tmp.end());
    std::stringstream ss;
    for (std::vector<int>::iterator i = tmp.begin(); i != tmp.end(); ++i)
    {
      if (i == tmp.begin())
      {
        ss << *i;
      }
      else
      {
        ss << " " << *i;
      }
    }
    return ss.str();
  }
};

extern "C" void register_services()
{
  std::cout << "C++ bundle registering complex service" << std::endl;
  CppRegistry::instance()->registerService<IComplexService>(new ComplexService);
}
