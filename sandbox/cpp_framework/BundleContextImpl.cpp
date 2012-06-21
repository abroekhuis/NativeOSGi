
#include "BundleContextImpl.h"

#include "ServiceRegistry.h"

#include <osgi/ServiceReference.h>
#include <osgi/ServiceRegistrationBase.h>
#include <osgi/InvalidSyntaxException.h>

// mock ups
namespace osgi {
  struct Bundle {};
  struct Filter {};
}
class BundleImpl : public osgi::Bundle {};

void BundleContextImpl::checkValidity() const
{
  if (m_valid)
  {
    return;
    //TODO use bundle implemenation
//    switch (m_bundle->getState())
//    {
//    case Bundle::ACTIVE:
//    case Bundle::STARTING:
//    case Bundle::STOPPING:
//      return;
//    }
  }

  throw osgi::IllegalStateException("Invalid BundleContext.");
}

osgi::ServiceReference<void> BundleContextImpl::getBestServiceReference(
    const std::vector<osgi::ServiceReference<void> >& refs, bool cppOnly)
{
  if (refs.empty())
  {
    return osgi::ServiceReference<void>();
  }

  // Loop through all service references and return
  // the "best" one according to its rank and ID.
  if (cppOnly)
  {
    osgi::ServiceReference<void> bestRef;
    for (int i = 0; i < refs.size(); i++)
    {
      if (!bestRef)
      {
        if (refs[i].isCppReference())
        {
          bestRef = refs[i];
        }
        else
        {
          continue;
        }
      }
      else if (refs[i].isCppReference() && bestRef < refs[i])
      {
        bestRef = refs[i];
      }
    }
    return bestRef;
  }
  else
  {
    osgi::ServiceReference<void> bestRef;
    for (int i = 0; i < refs.size(); i++)
    {
      if (!bestRef)
      {
        if (!refs[i].isCppReference())
        {
          bestRef = refs[i];
        }
        else
        {
          continue;
        }
      }
      else if (!refs[i].isCppReference() && bestRef < refs[i])
      {
        bestRef = refs[i];
      }
    }
    return bestRef;
  }
}

void *BundleContextImpl::getService(const osgi::ServiceReferenceBase& ref)
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation,
  // but we ignore it since the time window is small and
  // the result is the same as if the calling thread had
  // won the race condition.

  return m_fw->getService(m_bundle, ref);
}

std::vector<osgi::ServiceReference<void> >
BundleContextImpl::getServiceReferences(const std::string& clazz,
                                        const std::string& filter)
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation,
  // but we ignore it since the time window is small and
  // the result is the same as if the calling thread had
  // won the race condition.

  //return m_fw->getServiceReferences(m_bundle, clazz, filter);
  return m_fw->getServiceReferences(clazz, osgi::Filter());
}

osgi::ServiceReference<void> BundleContextImpl::getServiceReference(const std::string& clazz, bool cppOnly)
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation,
  // but we ignore it since the time window is small and
  // the result is the same as if the calling thread had
  // won the race condition.

  try
  {
    return getBestServiceReference(getServiceReferences(clazz, std::string()), cppOnly);
  }
  catch (const osgi::InvalidSyntaxException& ex)
  {
    // TODO logging
    //m_logger.log(m_bundle, Logger.LOG_ERROR, "BundleContextImpl: " + ex);
  }
  return osgi::ServiceReference<void>();
}

osgi::ServiceRegistrationBase BundleContextImpl::registerService(const std::map<std::string,void*>& servicePointers,
                                                                 bool isNativeCpp, const osgi::ServiceProperties& properties)
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation,
  // but we ignore it since the time window is small and
  // the result is the same as if the calling thread had
  // won the race condition.

  return m_fw->registerService(m_bundle, servicePointers, isNativeCpp, properties);
}

BundleContextImpl::BundleContextImpl(/*Logger logger, */FrameworkImpl* fw, BundleImpl* bundle)
  : m_fw(fw), m_bundle(bundle), m_valid(true)
{
  //m_logger = logger;
}

void BundleContextImpl::invalidate()
{
  m_valid = false;
}

void BundleContextImpl::addRequirement(const std::string& s)
{
  throw osgi::BundleException("Not implemented yet.");
}

void BundleContextImpl::removeRequirement()
{
  throw osgi::BundleException("Not implemented yet.");
}

void BundleContextImpl::addCapability()
{
  throw osgi::BundleException("Not implemented yet.");
}

void BundleContextImpl::removeCapability()
{
  throw osgi::BundleException("Not implemented yet.");
}

std::string BundleContextImpl::getProperty(const std::string& name) const
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation,
  // but we ignore it since the time window is small and
  // the result is the same as if the calling thread had
  // won the race condition.

  //return m_fw->getProperty(name);
  return "";
}

osgi::Bundle* BundleContextImpl::getBundle() const
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation,
  // but we ignore it since the time window is small and
  // the result is the same as if the calling thread had
  // won the race condition.

  return m_bundle;
}

osgi::Filter BundleContextImpl::createFilter(const std::string &expr) const
{
  return osgi::Filter();
}

//osgi::Filter BundleContextImpl::createFilter(const std::string& expr) const
//{
//  checkValidity();

//  // CONCURRENCY NOTE: This is a check-then-act situation,
//  // but we ignore it since the time window is small and
//  // the result is the same as if the calling thread had
//  // won the race condition.

//  return new FilterImpl(expr);
//}

osgi::Bundle* BundleContextImpl::installBundle(const std::string& location, std::istream* is)
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation,
  // but we ignore it since the time window is small and
  // the result is the same as if the calling thread had
  // won the race condition.

  //return m_fw->installBundle(m_bundle, location, is);
  return 0;
}

osgi::Bundle* BundleContextImpl::getBundle(long id) const
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation,
  // but we ignore it since the time window is small and
  // the result is the same as if the calling thread had
  // won the race condition.

  //return m_fw->getBundle(this, id);
  return 0;
}

osgi::Bundle* BundleContextImpl::getBundle(const std::string& location) const
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation,
  // but we ignore it since the time window is small and
  // the result is the same as if the calling thread had
  // won the race condition.

  //return m_fw->getBundle(location);
  return 0;
}

std::vector<osgi::Bundle*> BundleContextImpl::getBundles() const
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation,
  // but we ignore it since the time window is small and
  // the result is the same as if the calling thread had
  // won the race condition.

  //return m_fw->getBundles(this);
  return std::vector<osgi::Bundle*>();
}

void BundleContextImpl::addBundleListener(const osgi::BundleListener& l)
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation, but
  // internally the event dispatcher double checks whether or not
  // the bundle context is valid before adding the service listener
  // while holding the event queue lock, so it will either succeed
  // or fail.

  //m_fw->addBundleListener(m_bundle, l);
}

void BundleContextImpl::removeBundleListener(const osgi::BundleListener& l)
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation,
  // but we ignore it since the time window is small and
  // the result is the same as if the calling thread had
  // won the race condition.

  //m_fw->removeBundleListener(m_bundle, l);
}

void BundleContextImpl::addServiceListener(const osgi::ServiceListener& l, const std::string& s)
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation, but
  // internally the event dispatcher double checks whether or not
  // the bundle context is valid before adding the service listener
  // while holding the event queue lock, so it will either succeed
  // or fail.

  //m_fw->addServiceListener(m_bundle, l, s);
}

void BundleContextImpl::removeServiceListener(const osgi::ServiceListener& l)
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation,
  // but we ignore it since the time window is small and
  // the result is the same as if the calling thread had
  // won the race condition.

  //m_fw->removeServiceListener(m_bundle, l);
}

void BundleContextImpl::addFrameworkListener(const osgi::FrameworkListener& l)
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation, but
  // internally the event dispatcher double checks whether or not
  // the bundle context is valid before adding the service listener
  // while holding the event queue lock, so it will either succeed
  // or fail.

  //m_fw->addFrameworkListener(m_bundle, l);
}

void BundleContextImpl::removeFrameworkListener(const osgi::FrameworkListener& l)
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation,
  // but we ignore it since the time window is small and
  // the result is the same as if the calling thread had
  // won the race condition.

  //m_fw->removeFrameworkListener(m_bundle, l);
}

bool BundleContextImpl::ungetService(const osgi::ServiceReferenceBase& ref) const
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation,
  // but we ignore it since the time window is small and
  // the result is the same as if the calling thread had
  // won the race condition.

  if (!ref)
  {
    throw osgi::InvalidArgumentException("Specified service reference is invalid.");
  }

  // Unget the specified service.
  //return m_fw->ungetService(m_bundle, ref);
  return false;
}

std::string BundleContextImpl::getDataFile(const std::string& s) const
{
  checkValidity();

  // CONCURRENCY NOTE: This is a check-then-act situation,
  // but we ignore it since the time window is small and
  // the result is the same as if the calling thread had
  // won the race condition.

  //return m_fw->getDataFile(m_bundle, s);
  return "";
}
