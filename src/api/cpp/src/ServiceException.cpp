
#include <osgi/ServiceException.h>

namespace osgi {

ServiceException::ServiceException(const std::string& msg, Type type)
  : RuntimeException(msg),
    f_Type(type)
{
}

ServiceException::ServiceException(const std::string& msg, Type type,
                                   const Exception& cause)
  : RuntimeException(msg, cause),
    f_Type(type)
{
}

ServiceException::ServiceException(const std::string& msg, const Exception& cause)
  : RuntimeException(msg, cause),
    f_Type(UNSPECIFIED)
{
}

ServiceException::ServiceException(const ServiceException& o)
  : RuntimeException(o), f_Type(o.f_Type)
{

}

ServiceException& ServiceException::operator=(const ServiceException& o)
{
  RuntimeException::operator=(o);
  f_Type = o.f_Type;
  return *this;
}

ServiceException::~ServiceException() throw()
{
}

const char* ServiceException::name() const throw()
{
  return "Service Exception";
}

ServiceException* ServiceException::clone() const
{
  return new ServiceException(*this);
}

ServiceException::Type ServiceException::getType() const
{
  return f_Type;
}

} // end namespace osgi

