
#include <osgi/ServiceReferenceBase.h>

#include <osgi/Any.h>
#include <osgi/Constants.h>

#include <cassert>

namespace osgi {

ServiceReferenceBaseImpl::~ServiceReferenceBaseImpl()
{
}

ServiceReferenceBase::ServiceReferenceBase()
{
}

ServiceReferenceBase::ServiceReferenceBase(ServiceReferenceBaseImpl* impl)
  : d(impl)
{
}

std::string ServiceReferenceBase::getInterfaceId() const
{
  assert(d);
  return d->getInterfaceId();
}

bool ServiceReferenceBase::isCppReference() const
{
  assert(d);
  return d->isCppReference();
}

Any ServiceReferenceBase::getProperty(const std::string& key) const
{
  assert(d);
  return d->getProperty(key);
}

std::vector<std::string> ServiceReferenceBase::getPropertyKeys() const
{
  assert(d);
  return d->getPropertyKeys();
}

Bundle* ServiceReferenceBase::getBundle() const
{
  return 0;
}

std::vector<Bundle*> ServiceReferenceBase::getUsingBundles() const
{
  return std::vector<Bundle*>();
}

bool ServiceReferenceBase::operator<(const ServiceReferenceBase& reference) const
{
  const long int id = any_cast<long int>(getProperty(Constants::SERVICE_ID()));
  const long int otherId = any_cast<long int>(reference.getProperty(Constants::SERVICE_ID()));

  if (id == otherId)
  {
    return false; // same service
  }

  // If no rank, then spec says it defaults to zero.
  int rank = 0;
  int otherRank = 0;

  Any rankObj = getProperty(Constants::SERVICE_RANKING());
  Any otherRankObj = reference.getProperty(Constants::SERVICE_RANKING());
  if (rankObj.type() == typeid(int)) rank = any_cast<int>(rankObj);
  if (otherRankObj.type() == typeid(int)) otherRank = any_cast<int>(otherRankObj);

  if (rank != otherRank)
  {
    // use ranking if ranking differs
    return rank < otherRank;
  }
  else
  {
    // If ranks are equal, then sort by service id in descending order.
    return otherId < id;
  }
}

bool ServiceReferenceBase::operator==(const ServiceReferenceBase& reference) const
{
  if (!d)
  {
    return reference.d ? false : true;
  }

  if (!reference.d) return false;
  return d == reference.d;
}

osgi::ServiceReferenceBase::operator bool() const
{
  return static_cast<bool>(d);
}

std::size_t ServiceReferenceBase::hash() const
{
  assert(d);
  return d->hash();
}

osgi::ServiceReferenceBase::operator ServiceReferenceBaseImpl *() const
{
  return d.data();
}

} // end namespace osgi

OSGI_CPP_EXPORT std::ostream& operator<<(std::ostream& os, const osgi::ServiceReferenceBase& serviceRef)
{
  os << "Reference for service object registered from "
//     << serviceRef.getBundle()->getName() << " " << serviceRef.getBundle()->getVersion()
     << " (";
  std::vector<std::string> keys = serviceRef.getPropertyKeys();
  int keySize = keys.size();
  for(int i = 0; i < keySize; ++i)
  {
    os << keys[i] << "=" << serviceRef.getProperty(keys[i]);
    if (i < keySize-1) os << ",";
  }
  os << ")";

  return os;
}
