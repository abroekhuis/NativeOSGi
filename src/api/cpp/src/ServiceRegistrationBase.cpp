
#include <osgi/ServiceRegistrationBase.h>
#include <osgi/ServiceReferenceBase.h>

#include <cassert>

namespace osgi {

ServiceRegistrationBaseImpl::~ServiceRegistrationBaseImpl()
{
}


ServiceRegistrationBase::ServiceRegistrationBase(ServiceRegistrationBaseImpl* impl)
  : d(impl)
{
}

ServiceRegistrationBase &ServiceRegistrationBase::operator =(int null)
{
  if (null == 0)
  {
    d.reset();
  }
  return *this;
}

bool ServiceRegistrationBase::operator <(const ServiceRegistrationBase &o) const
{
  if (!d || !o) return true;
  return d.data() < o.d.data();
}

bool ServiceRegistrationBase::operator ==(const ServiceRegistrationBase &o) const
{
  return d == o.d;
}

osgi::ServiceRegistrationBase::operator bool() const
{
  return static_cast<bool>(d);
}

ServiceReferenceBase ServiceRegistrationBase::getReference(const std::string &interfaceId, bool cpp) const
{
  assert(d);
  return d->getReference(interfaceId, cpp);
}

void ServiceRegistrationBase::setProperties(const ServiceProperties &properties)
{
  assert(d);
  d->setProperties(properties);
}

void ServiceRegistrationBase::unregister()
{
  assert(d);
  d->unregister();
}


} // end namespace osgi
