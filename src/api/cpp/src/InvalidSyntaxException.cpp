
#include <osgi/InvalidSyntaxException.h>

namespace osgi {

InvalidSyntaxException::InvalidSyntaxException(const std::string& msg, const std::string& filter)
  : Exception(msg), f_Filter(filter)
{
}

InvalidSyntaxException::InvalidSyntaxException(const std::string& msg, const std::string& filter, const Exception& cause)
  : Exception(msg, cause), f_Filter(filter)
{
}

std::string InvalidSyntaxException::getFilter() const
{
  return f_Filter;
}

InvalidSyntaxException::InvalidSyntaxException(const InvalidSyntaxException& o)
  : Exception(o), f_Filter(o.f_Filter)
{
}

InvalidSyntaxException& InvalidSyntaxException::operator=(const InvalidSyntaxException& o)
{
  Exception::operator=(o);
  f_Filter = o.f_Filter;
  return *this;
}

InvalidSyntaxException::~InvalidSyntaxException() throw()
{
}

const char* InvalidSyntaxException::name() const throw()
{
  return "Invalid Syntax Exception";
}

InvalidSyntaxException* InvalidSyntaxException::clone() const
{
  return new InvalidSyntaxException(*this);
}

} // end namespace osgi

