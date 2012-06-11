
#include <osgi/Exception.h>

#include <typeinfo>
#include <ostream>

namespace osgi {

Exception::Exception(const std::string& msg)
  : f_Msg(msg), f_NestedException(0)
{
}

Exception::Exception(const std::string& msg, const Exception& cause)
  : f_Msg(msg), f_NestedException(cause.clone())
{
}

Exception::Exception(const Exception& exc)
  : std::exception(exc), f_Msg(exc.f_Msg)
{
  f_NestedException = exc.f_NestedException ? exc.f_NestedException->clone() : 0;
}

Exception::~Exception() throw()
{
  delete f_NestedException;
}

Exception& Exception::operator=(const Exception& exc)
{
  if (&exc != this)
  {
    delete f_NestedException;
    f_Msg = exc.f_Msg;
    f_NestedException = exc.f_NestedException ? exc.f_NestedException->clone() : 0;
  }
  return *this;
}

const Exception* Exception::cause() const throw()
{
  return f_NestedException;
}

void Exception::setCause(const Exception& cause)
{
  delete f_NestedException;
  f_NestedException = cause.clone();
}

const char *Exception::name() const throw()
{
  return "Exception";
}

const char* Exception::className() const throw()
{
  return typeid(*this).name();
}

const char* Exception::what() const throw()
{
  static std::string txt;
  txt = std::string(name());
  if (!f_Msg.empty())
  {
    txt += ": ";
    txt += f_Msg;
  }
  if (f_NestedException)
  {
    txt +=  std::string("\n  Caused by: ") + f_NestedException->what();
  }
  return txt.c_str();
}

Exception* Exception::clone() const
{
  return new Exception(*this);
}

void Exception::rethrow() const
{
  throw *this;
}

OSGI_IMPLEMENT_EXCEPTION(RuntimeException, Exception, "Runtime Exception")
OSGI_IMPLEMENT_EXCEPTION(InvalidArgumentException, RuntimeException, "Invalid Argument Exception")
OSGI_IMPLEMENT_EXCEPTION(IllegalStateException, RuntimeException, "Illegal State Exception")

} // end namespace osgi

std::ostream& operator<<(std::ostream& os, const osgi::Exception& exc)
{
  return os << exc.what();
}
