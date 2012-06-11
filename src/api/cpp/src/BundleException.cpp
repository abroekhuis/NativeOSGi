
#include <osgi/BundleException.h>

namespace osgi {

BundleException::BundleException(const std::string& msg, const Exception& cause)
  : Exception(msg, cause),
    f_Type(UNSPECIFIED)
{
}

BundleException::BundleException(const std::string& msg, Type type, const Exception& cause)
  : Exception(msg, cause),
    f_Type(type)
{
}

BundleException::BundleException(const std::string& msg, Type type)
  : Exception(msg),
    f_Type(type)
{
}

BundleException::~BundleException() throw()
{
}

const char* BundleException::name() const throw()
{
  return "Bundle Exception";
}

BundleException* BundleException::clone() const
{
  return new BundleException(*this);
}

BundleException::Type BundleException::getType() const
{
  return f_Type;
}

} // end namespace osgi
