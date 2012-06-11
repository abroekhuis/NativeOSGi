
#include <osgi/Constants.h>

namespace osgi {

const std::string& Constants::OBJECTCLASS()
{
  static std::string s = "objectClass";
  return s;
}

const std::string& Constants::OBJECTCLASS_BINDINGS()
{
  static std::string s = "objectClass.bindings";
  return s;
}

const std::string& Constants::SERVICE_ID()
{
  static std::string s = "service.id";
  return s;
}

const std::string& Constants::SERVICE_PID()
{
  static std::string s = "service.pid";
  return s;
}

const std::string& Constants::SERVICE_RANKING()
{
  static std::string s = "service.ranking";
  return s;
}

const std::string& Constants::SERVICE_VENDOR()
{
  static std::string s = "service.vendor";
  return s;
}

const std::string& Constants::SERVICE_DESCRIPTION()
{
  static std::string s = "service.description";
  return s;
}

}
