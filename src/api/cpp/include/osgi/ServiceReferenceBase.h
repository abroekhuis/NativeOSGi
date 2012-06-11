
#pragma once
#ifndef OSGI_SERVICEREFERENCEBASE_H_
#define OSGI_SERVICEREFERENCEBASE_H_

#include <osgi/Config.h>
#include <osgi/Export.h>
#include <osgi/SharedData.h>

#include <vector>
#include <ostream>

namespace osgi {

class Any;

struct Bundle;

/**
 * A reference to a service.
 */
struct ServiceReferenceBaseImpl : public SharedData
{

  virtual ~ServiceReferenceBaseImpl();

  virtual std::string getInterfaceId() const = 0;

  virtual bool isCppReference() const = 0;

  virtual Any getProperty(const std::string& key) const = 0;

  virtual std::vector<std::string> getPropertyKeys() const = 0;

  virtual Bundle* getBundle() const = 0;

  virtual std::vector<Bundle*> getUsingBundles() const = 0;

  virtual std::size_t hash() const = 0;

};

/**
 * A reference to a service.
 */
class ServiceReferenceBase
{

public:

  ServiceReferenceBase();

  ServiceReferenceBase(ServiceReferenceBaseImpl* impl);

  std::string getInterfaceId() const;

  bool isCppReference() const;

  Any getProperty(const std::string& key) const;

  std::vector<std::string> getPropertyKeys() const;

  Bundle* getBundle() const;

  std::vector<Bundle*> getUsingBundles() const;

  /**
   * Compares this <code>ServiceReference</code> with the specified
   * <code>ServiceReference</code> for order.
   *
   * <p>
   * If this <code>ServiceReference</code> and the specified
   * <code>ServiceReference</code> have the same \link ServiceConstants::SERVICE_ID()
   * service id\endlink they are equal. This <code>ServiceReference</code> is less
   * than the specified <code>ServiceReference</code> if it has a lower
   * {@link ServiceConstants::SERVICE_RANKING service ranking} and greater if it has a
   * higher service ranking. Otherwise, if this <code>ServiceReference</code>
   * and the specified <code>ServiceReference</code> have the same
   * {@link ServiceConstants::SERVICE_RANKING service ranking}, this
   * <code>ServiceReference</code> is less than the specified
   * <code>ServiceReference</code> if it has a higher
   * {@link ServiceConstants::SERVICE_ID service id} and greater if it has a lower
   * service id.
   *
   * @param reference The <code>ServiceReference</code> to be compared.
   * @return Returns a false or true if this
   *         <code>ServiceReference</code> is less than or greater
   *         than the specified <code>ServiceReference</code>.
   */
  bool operator<(const ServiceReferenceBase& reference) const;

  bool operator==(const ServiceReferenceBase& reference) const;

  operator bool() const;

  operator ServiceReferenceBaseImpl*() const;

private:

  OSGI_HASH_FUNCTION_FRIEND(ServiceReferenceBase);

  std::size_t hash() const;

  ExplicitlySharedDataPointer<ServiceReferenceBaseImpl> d;

};

} // end namespace osgi

OSGI_CPP_EXPORT std::ostream& operator<<(std::ostream& os, const osgi::ServiceReferenceBase& serviceRef);

OSGI_HASH_FUNCTION_NAMESPACE_BEGIN
OSGI_HASH_FUNCTION_BEGIN(osgi::ServiceReferenceBase)
  return arg.hash();
OSGI_HASH_FUNCTION_END
OSGI_HASH_FUNCTION_NAMESPACE_END

#endif // OSGI_SERVICEREFERENCEBASE_H_
