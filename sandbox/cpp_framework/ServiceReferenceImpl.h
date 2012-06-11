
#pragma once
#ifndef SERVICEREFERENCEIMPL_H
#define SERVICEREFERENCEIMPL_H

#include <osgi/ServiceReferenceBase.h>

class ServiceRegistrationImpl;

class ServiceReferenceImpl : public osgi::ServiceReferenceBaseImpl
{

private:

  std::string f_InterfaceId;
  ServiceRegistrationImpl* f_ServiceRegistration;
  bool f_IsCpp;

public:

  ServiceReferenceImpl(const std::string& interfaceId, bool isCpp, ServiceRegistrationImpl* serviceReg);

  ServiceRegistrationImpl* getRegistration() const;


  //
  // ServiceReferenceBaseImpl methods.
  //

  std::string getInterfaceId() const;

  bool isCppReference() const;

  osgi::Any getProperty(const std::string& s) const;

  std::vector<std::string> getPropertyKeys() const;

  osgi::Bundle* getBundle() const;

  std::vector<osgi::Bundle*> getUsingBundles() const;

  std::string toString() const;

  std::size_t hash() const;

};

#endif // SERVICEREFERENCEIMPL_H
