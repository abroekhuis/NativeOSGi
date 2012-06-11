
#include <osgi/Any.h>

namespace osgi {

template<typename Iterator>
std::string container_to_string(Iterator i1, Iterator i2)
{
  std::stringstream ss;
  ss << "(";
  const Iterator begin = i1;
  for ( ; i1 != i2; ++i1)
  {
    if (i1 == begin) ss << *i1;
    else ss << "," << *i1;
  }
  ss << ")";
  return ss.str();
}

std::string any_value_to_string(const std::vector<std::string>& val)
{
  return container_to_string(val.begin(), val.end());
}

std::string any_value_to_string(const std::list<std::string>& val)
{
  return container_to_string(val.begin(), val.end());
}

std::string any_value_to_string(const std::vector<Any>& val)
{
  return container_to_string(val.begin(), val.end());
}

} // end namespace osgi
