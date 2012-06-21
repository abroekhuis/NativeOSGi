/*=============================================================================

  Copyright (c) Sascha Zelzer

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/

#pragma once
#ifndef OSGI_BUNDLECONTEXT_H_
#define OSGI_BUNDLECONTEXT_H_


//typedef US_SERVICE_LISTENER_FUNCTOR ServiceListener;
//typedef US_MODULE_LISTENER_FUNCTOR BundleListener;

#include <osgi/ServiceProperties.h>

#include <vector>
#include <string>
#include <map>

namespace osgi {

struct Bundle;
class ServiceReferenceBase;
class ServiceRegistrationBase;
class ServiceListener;
class BundleListener;
class FrameworkListener;
class ServiceEvent;
class BundleEvent;
class FrameworkEvent;
class Filter;

template<class S>
class ServiceReference;

#ifndef _ServiceRegistrationTemplate_forward_declared_
#define _ServiceRegistrationTemplate_forward_declared_
template<class S, class T = void> class ServiceRegistration;
#endif


/**
 * \ingroup cpp_api
 *
 * A bundle's execution context within the framework. The context is used to
 * grant access to other methods so that this bundle can interact with the
 * Framework.
 *
 * <p>
 * {@code BundleContext} methods allow a bundle to:
 * <ul>
 * <li>Subscribe to events published by the Framework.
 * <li>Register service objects with the Framework service registry.
 * <li>Retrieve {@code ServiceReferences} from the Framework service registry.
 * <li>Get and release service objects for a referenced service.
 * <li>Install new bundles in the Framework.
 * <li>Get the list of bundles installed in the Framework.
 * <li>Get the {@link Bundle} object for a bundle.
 * <li>Create {@code File} objects for files in a persistent storage area
 * provided for the bundle by the Framework.
 * </ul>
 *
* <p>
 * A {@code BundleContext} object will be created for a bundle when the bundle
 * is started. The {@code Bundle} object associated with a {@code BundleContext}
 * object is called the <em>context bundle</em>.
 *
 * <p>
 * The {@code BundleContext} object will be passed to the
 * {@link BundleActivator#start} method during activation of the context bundle.
 * The same {@code BundleContext} object will be passed to the
 * {@link BundleActivator#stop} method when the context bundle is stopped. A
 * {@code BundleContext} object is generally for the private use of its
 * associated bundle and is not meant to be shared with other bundles in the
 * OSGi environment.
 *
 * <p>
 * The {@code BundleContext} object is only valid during the execution of its
 * context bundle; that is, during the period from when the context bundle is in
 * the {@code STARTING}, {@code STOPPING}, and {@code ACTIVE} bundle states. If
 * the {@code BundleContext} object is used subsequently, an
 * {@code IllegalStateException} must be thrown. The {@code BundleContext}
 * object must never be reused after its context bundle is stopped.
 *
 * <p>
 * Two {@code BundleContext} objects are equal if they both refer to the same
 * execution context of a bundle. The Framework is the only entity that can
 * create {@code BundleContext} objects and they are only valid within the
 * Framework that created them.
 *
 * <p>
 * A {@link Bundle} can be {@link Bundle#adapt(Class) adapted} to its
 * {@code BundleContext}.
 *
 * @threadsafe
 * @noimplement
 */
struct BundleContext
{

private:

  /**
   * Returns the service object referenced by the specified
   * \c ServiceReferenceBase object.
   *
   * This method is used by the templated getService(ServiceReference)
   * method and is required to return a void* pointer which can be
   * cast to the template argument of the ServiceReference object which
   * was originally used to wrap the \c reference argument.
   */
  virtual void* getService(const ServiceReferenceBase& reference) = 0;

public:

  virtual ~BundleContext();

  /**
   * Returns the value of the specified property. If the key is not found in
   * the Framework properties, the system properties are then searched. The
   * method returns {@code null} if the property is not found.
   *
   * @param key The name of the requested property.
   * @return The value of the requested property, or {@code null} if the
   *         property is undefined.
   */
  virtual std::string getProperty(const std::string& key) const = 0;

  /**
   * Returns the <code>IBundle</code> object associated with this
   * <code>BundleContext</code>. This bundle is called the context bundle.
   *
   * @return The <code>IBundle</code> object associated with this
   *         <code>BundleContext</code>.
   * @throws IllegalStateException If this BundleContext is no
   *         longer valid.
   */
  virtual Bundle* getBundle() const = 0;

  /**
   * Installs a bundle from the specified {@code std::istream} object.
   *
   * <p>
   * If the specified {@code std::istream} is {@code NULL}, the Framework must
   * create the {@code std::istream} from which to read the bundle by
   * interpreting, in an implementation dependent manner, the specified
   * {@code location}.
   *
   * <p>
   * The specified {@code location} identifier will be used as the identity of
   * the bundle. Every installed bundle is uniquely identified by its location
   * identifier which is typically in the form of a URL.
   *
   * <p>
   * The following steps are required to install a bundle:
   * <ol>
   * <li>If a bundle containing the same location identifier is already
   * installed, the {@code Bundle} object for that bundle is returned.
   *
   * <li>The bundle's content is read from the input stream. If this fails, a
   * {@link BundleException} is thrown.
   *
   * <li>The bundle's associated resources are allocated. The associated
   * resources minimally consist of a unique identifier and a persistent
   * storage area if the platform has file system support. If this step fails,
   * a {@code BundleException} is thrown.
   *
   * <li>The bundle's state is set to {@code INSTALLED}.
   *
   * <li>A bundle event of type {@link BundleEvent#INSTALLED} is fired.
   *
   * <li>The {@code Bundle} object for the newly or previously installed
   * bundle is returned.
   * </ol>
   *
   * <b>Postconditions, no exceptions thrown </b>
   * <ul>
   * <li>{@code getState()} in &#x007B; {@code INSTALLED}, {@code RESOLVED}
   * &#x007D;.
   * <li>Bundle has a unique ID.
   * </ul>
   * <b>Postconditions, when an exception is thrown </b>
   * <ul>
   * <li>Bundle is not installed. If there was an existing bundle for the
   * specified location, then that bundle must still be in the state it was
   * prior to calling this method.</li>
   * </ul>
   *
   * @param location The location identifier of the bundle to install.
   * @param input The {@code std::istream} object from which this bundle will
   *        be read or {@code NULL} to indicate the Framework must create the
   *        input stream from the specified location identifier. The input
   *        stream must always be closed when this method completes, even if
   *        an exception is thrown.
   * @return The {@code Bundle} object of the installed bundle.
   * @throws BundleException If the installation failed. BundleException types
   *         thrown by this method include: {@link BundleException#READ_ERROR}
   *         , {@link BundleException#DUPLICATE_BUNDLE_ERROR},
   *         {@link BundleException#MANIFEST_ERROR}, and
   *         {@link BundleException#REJECTED_BY_HOOK}.
   * @throws IllegalStateException If this BundleContext is no longer valid.
   */
  virtual Bundle* installBundle(const std::string& location, std::istream* input = NULL) = 0;

  /**
   * Returns the bundle with the specified identifier.
   *
   * @param id The identifier of the bundle to retrieve.
   * @return A <code>IBundle</code> object or <code>NULL</code> if the
   *         identifier does not match any previously loaded bundle.
   */
  virtual Bundle* getBundle(long id) const = 0;

  /**
   * Returns a list of all known modules.
   * <p>
   * This method returns a list of all modules loaded in the bundle
   * environment at the time of the call to this method. This list will
   * also contain modules which might already have been unloaded.
   *
   * @param modules A std::vector of <code>IBundle</code> objects which
   *                will hold one object per known bundle.
   */
  virtual std::vector<Bundle*> getBundles() const = 0;

  /**
   * Returns a list of <code>ServiceReferenceBase</code> objects. The returned
   * list contains services that
   * were registered under the specified class and match the specified filter
   * expression.
   *
   * <p>
   * The list is valid at the time of the call to this method. However since
   * the Framework is a very dynamic environment, services can be modified or
   * unregistered at any time.
   *
   * <p>
   * The specified <code>filter</code> expression is used to select the
   * registered services whose service properties contain keys and values
   * which satisfy the filter expression. See LDAPFilter for a description
   * of the filter syntax. If the specified <code>filter</code> is
   * empty, all registered services are considered to match the
   * filter. If the specified <code>filter</code> expression cannot be parsed,
   * an <code>std::invalid_argument</code> will be thrown with a human readable
   * message where the filter became unparsable.
   *
   * <p>
   * The result is a list of <code>ServiceReferenceBase</code> objects for all
   * services that meet all of the following conditions:
   * <ul>
   * <li>If the specified class name, <code>clazz</code>, is not
   * empty, the service must have been registered with the
   * specified class name. The complete list of class names with which a
   * service was registered is available from the service's
   * {@link Constants#OBJECTCLASS() objectClass} property.
   * <li>If the specified <code>filter</code> is not empty, the
   * filter expression must match the service.
   * </ul>
   *
   * @param clazz The class name with which the service was registered or
   *        an empty string for all services.
   * @param filter The filter expression or empty for all
   *        services.
   * @return A list of <code>ServiceReferenceBase</code> objects or
   *         an empty list if no services are registered which satisfy the
   *         search.
   * @throws InvalidArgumentException If the specified <code>filter</code>
   *         contains an invalid filter expression that cannot be parsed.
   * @throws IllegalStateException If this BundleContext is no longer valid.
   */
  virtual std::vector<ServiceReference<void> > getServiceReferences(const std::string& clazz,
                                                                    const std::string& filter) = 0;

  /**
   * Returns a <code>ServiceReferenceBase</code> object for a service that
   * implements and was registered under the specified class.
   *
   * <p>
   * The returned <code>ServiceReferenceBase</code> object is valid at the time of
   * the call to this method. However as the Framework is a very dynamic
   * environment, services can be modified or unregistered at any time.
   *
   * <p>
   * This method is the same as calling
   * {@link BundleContext::getServiceReferences(const std::string&, const std::string&)} with an
   * empty filter expression. It is provided as a convenience for
   * when the caller is interested in any service that implements the
   * specified class.
   * <p>
   * If multiple such services exist, the service with the highest ranking (as
   * specified in its Constants::SERVICE_RANKING() property) is returned.
   * <p>
   * If there is a tie in ranking, the service with the lowest service ID (as
   * specified in its Constants::SERVICE_ID() property); that is, the
   * service that was registered first is returned.
   *
   * @param clazz The class name with which the service was registered.
   * @return A <code>ServiceReferenceBase</code> object, or an invalid <code>ServiceReferenceBase</code> if
   *         no services are registered which implement the named class.
   * @throws IllegalStateException If this BundleContext is no longer valid.
   * @throws ServiceException If no service was registered under the given class name.
   * @see #getServiceReferences(const std::string&, const std::string&)
   */
  virtual ServiceReference<void> getServiceReference(const std::string& clazz, bool cppOnly = true) = 0;

  /**
   * Registers the specified service object with the specified properties
   * under the specified class names into the framework. A
   * <code>ServiceRegistrationBase</code> object is returned. The
   * <code>ServiceRegistrationBase</code> object is for the private use of the
   * bundle registering the service and should not be shared with other
   * modules. The registering bundle is defined to be the context bundle.
   * Other modules can locate the service by using either the
   * {@link #getServiceReferences} or {@link #getServiceReference} method.
   *
   * <p>
   * A bundle can register a service object that implements the
   * {@link ServiceFactory} interface to have more flexibility in providing
   * service objects to other modules.
   *
   * <p>
   * The following steps are taken when registering a service:
   * <ol>
   * <li>The framework adds the following service properties to the service
   * properties from the specified <code>ServiceProperties</code> (which may be
   * omitted): <br/>
   * A property named Constants#SERVICE_ID() identifying the
   * registration number of the service <br/>
   * A property named Constants#OBJECTCLASS() containing all the
   * specified classes. <br/>
   * Properties with these names in the specified <code>ServiceProperties</code> will
   * be ignored.
   * <li>The service is added to the framework service registry and may now be
   * used by other modules.
   * <li>A service event of type {@link ServiceEvent#REGISTERED} is fired.
   * <li>A <code>ServiceRegistrationBase</code> object for this registration is
   * returned.
   * </ol>
   *
   * @param clazzes The class names under which the service can be located.
   *        The class names will be stored in the service's
   *        properties under the key Constants#OBJECTCLASS().
   * @param service The service object or a <code>ServiceFactory</code>
   *        object.
   * @param properties The properties for this service. The keys in the
   *        properties object must all be <code>std::string</code> objects. See
   *        {@link Constants} for a list of standard service property keys.
   *        Changes should not be made to this object after calling this
   *        method. To update the service's properties the
   *        {@link ServiceRegistration::setProperties} method must be called.
   *        The set of properties may be omitted if the service has
   *        no properties.
   * @return A <code>ServiceRegistration</code> object for use by the bundle
   *         registering the service to update the service's properties or to
   *         unregister the service.
   * @throws InvalidArgumentException If one of the following is true:
   *         <ul>
   *         <li><code>service</code> is <code>0</code>.
   *         <li><code>properties</code> contains case variants of the same key name.
   *         </ul>
   * @throws IllegalStateException If this BundleContext is no longer valid.
   * @see ServiceRegistration
   * @see ServiceFactory
   */
  virtual ServiceRegistrationBase registerService(const std::map<std::string, void*>& service,
                                                  bool isNativeCpp,
                                                  const ServiceProperties& properties) = 0;

  /**
   * Registers the specified service object with the specified properties
   * under the specified template argument type into the Framework. A
   * {@code ServiceRegistration} object is returned. The
   * {@code ServiceRegistration} object is for the private use of the bundle
   * registering the service and should not be shared with other bundles. The
   * registering bundle is defined to be the context bundle. Other bundles can
   * locate the service by using either the {@link #getServiceReferences} or
   * {@link #getServiceReference} method.
   *
   * <p>
   * A bundle can register a service object that implements the
   * {@link ServiceFactory} interface to have more flexibility in providing
   * service objects to other bundles.
   *
   * <p>
   * The following steps are required to register a service:
   * <ol>
   * <li>If {@code service} is not a {@code ServiceFactory}, an
   * {@code InvalidArgumentException} is thrown if {@code service} is not an
   * {@code instanceof} all the specified class names.
   * <li>The Framework adds the following service properties to the service
   * properties from the specified {@code ServiceProperties} (which may be
   * {@code null}): <br/>
   * A property named {@link Constants#SERVICE_ID} identifying the
   * registration number of the service <br/>
   * A property named {@link Constants#OBJECTCLASS} containing all the
   * specified classes. <br/>
   * Properties with these names in the specified {@code ServiceProperties} will be
   * ignored.
   * <li>The service is added to the Framework service registry and may now be
   * used by other bundles.
   * <li>A service event of type {@link ServiceEvent#REGISTERED} is fired.
   * <li>A {@code ServiceRegistration} object for this registration is
   * returned.
   * </ol>
   *
   * @param clazzes The class names under which the service can be located.
   *        The class names in this array will be stored in the service's
   *        properties under the key {@link Constants#OBJECTCLASS}.
   * @param service The service object or a {@code ServiceFactory} object.
   * @param properties The properties for this service. The keys in the
   *        properties object must all be {@code String} objects. See
   *        {@link Constants} for a list of standard service property keys.
   *        Changes should not be made to this object after calling this
   *        method. To update the service's properties the
   *        {@link ServiceRegistration#setProperties} method must be called.
   *        The set of properties may be {@code null} if the service has no
   *        properties.
   * @return A {@code ServiceRegistration} object for use by the bundle
   *         registering the service to update the service's properties or to
   *         unregister the service.
   * @throws InvalidArgumentException If one of the following is true:
   *         <ul>
   *         <li>{@code service} is {@code null}. <li>{@code service} is not a
   *         {@code ServiceFactory} object and is not an instance of all the
   *         named classes in {@code clazzes}. <li> {@code properties}
   *         contains case variants of the same key name.
   *         </ul>
   * @throws IllegalStateException If this BundleContext is no longer valid.
   * @see ServiceRegistration
   * @see ServiceFactory
   */
  template<class S>
  ServiceRegistration<S> registerService(S* service, const ServiceProperties& properties = ServiceProperties());

  template<class S, class T>
  ServiceRegistration<S,T> registerService(S* srvIFace1, T* srvIFace2, const ServiceProperties& properties = ServiceProperties());

  /**
   * Returns an array of {@code ServiceReference} objects. The returned array
   * of {@code ServiceReference} objects contains services that were
   * registered under the specified class and match the specified filter
   * expression.
   *
   * <p>
   * The list is valid at the time of the call to this method. However since
   * the Framework is a very dynamic environment, services can be modified or
   * unregistered at any time.
   *
   * <p>
   * The specified {@code filter} expression is used to select the registered
   * services whose service properties contain keys and values which satisfy
   * the filter expression. See {@link Filter} for a description of the filter
   * syntax. If the specified {@code filter} is {@code null}, all registered
   * services are considered to match the filter. If the specified
   * {@code filter} expression cannot be parsed, an
   * {@link InvalidSyntaxException} will be thrown with a human readable
   * message where the filter became unparsable.
   *
   * <p>
   * The result is an array of {@code ServiceReference} objects for all
   * services that meet all of the following conditions:
   * <ul>
   * <li>If the specified class name, {@code clazz}, is not {@code null}, the
   * service must have been registered with the specified class name. The
   * complete list of class names with which a service was registered is
   * available from the service's {@link Constants#OBJECTCLASS objectClass}
   * property.
   * <li>If the specified {@code filter} is not empty, the filter
   * expression must match the service.
   * </ul>
   *
   * @tparam S The type with which the service was registered.
   * @param filter The filter expression or {@code empty} for all services.
   * @return An array of {@code ServiceReference} objects.
   * @throws InvalidSyntaxException If the specified {@code filter} contains
   *         an invalid filter expression that cannot be parsed.
   * @throws IllegalStateException If this BundleContext is no longer valid.
   */
  template<class S>
  std::vector<ServiceReference<S> > getServiceReferences(const std::string& filter = std::string());

  /**
   * Returns a {@code ServiceReference} object for a service that implements
   * and was registered under the specified type.
   *
   * <p>
   * The returned {@code ServiceReference} object is valid at the time of the
   * call to this method. However as the Framework is a very dynamic
   * environment, services can be modified or unregistered at any time.
   *
   * <p>
   * This method is the same as calling
   * {@link #getServiceReferences<S>(std::string)} with an empty filter
   * expression and then finding the reference with the highest priority. It
   * is provided as a convenience for when the caller is interested in any
   * service that implements the specified class.
   * <p>
   * If multiple such services exist, the service with the highest priority is
   * selected. This priority is defined as the service reference with the
   * highest ranking (as specified in its {@link Constants#SERVICE_RANKING}
   * property) is returned.
   * <p>
   * If there is a tie in ranking, the service with the lowest service ID (as
   * specified in its {@link Constants#SERVICE_ID} property); that is, the
   * service that was registered first is returned.
   *
   * @tparam S The type with which the service was registered.
   * @return A {@code ServiceReference} object. The returned object is invalid if no services
     *         are registered which implement the given type.
     * @throws IllegalStateException If this BundleContext is no longer valid.
     * @see #getServiceReferences<S>(std::string)
     */
  template<class S>
  ServiceReference<S> getServiceReference();

  /**
   * Returns the service object referenced by the specified
   * {@code ServiceReference} object.
   * <p>
   * A bundle's use of a service is tracked by the bundle's use count of that
   * service. Each time a service's service object is returned by
   * {@link #getService(ServiceReference)} the context bundle's use count for
   * that service is incremented by one. Each time the service is released by
   * {@link #ungetService(ServiceReference)} the context bundle's use count
   * for that service is decremented by one.
   * <p>
   * When a bundle's use count for a service drops to zero, the bundle should
   * no longer use that service.
   *
   * <p>
   * This method will always return {@code NULL} when the service associated
   * with this {@code reference} has been unregistered.
   *
   * <p>
   * The following steps are required to get the service object:
   * <ol>
   * <li>If the service has been unregistered, {@code NULL} is returned.
   * <li>If the context bundle's use count for the service is currently zero
   * and the service was registered with an object implementing the
   * {@code ServiceFactory} interface, the
   * {@link ServiceFactory#getService(Bundle, ServiceRegistration)} method is
   * called to create a service object for the context bundle. If the service
   * object returned by the {@code ServiceFactory} object is {@code NULL}, not
   * an instance of all the classes named when the service was
   * registered or the {@code ServiceFactory} object throws an exception or
   * will be recursively called for the context bundle, {@code NULL} is
   * returned and a Framework event of type {@link FrameworkEvent#ERROR}
   * containing a {@link ServiceException} describing the error is fired. <br>
   * This service object is cached by the Framework. While the context
   * bundle's use count for the service is greater than zero, subsequent calls
   * to get the services's service object for the context bundle will return
   * the cached service object.
   * <li>The context bundle's use count for this service is incremented by
   * one.
   * <li>The service object for the service is returned.
   * </ol>
   *
   * @tparam S Type of Service.
   * @param reference A reference to the service.
   * @return A service object for the service associated with
   *         {@code reference} or {@code NULL} if the service is not
   *         registered, the service object returned by a
   *         {@code ServiceFactory} does not implement the classes under which
   *         it was registered or the {@code ServiceFactory} threw an
   *         exception.
   * @throws IllegalStateException If this BundleContext is no longer valid.
   * @throws InvalidArgumentException If the specified
   *         {@code ServiceReference} was not created by the same framework
   *         instance as this {@code BundleContext}.
   * @see #ungetService(ServiceReference)
   * @see ServiceFactory
   */
  template<class S>
  S* getService(const ServiceReference<S>& reference);

  /**
   * Releases the service object referenced by the specified
   * <code>ServiceReference</code> object. If the context bundle's use count
   * for the service is zero, this method returns <code>false</code>.
   * Otherwise, the context modules's use count for the service is decremented
   * by one.
   *
   * <p>
   * The service's service object should no longer be used and all references
   * to it should be destroyed when a bundle's use count for the service drops
   * to zero.
   *
   * <p>
   * The following steps are taken to unget the service object:
   * <ol>
   * <li>If the context bundle's use count for the service is zero or the
   * service has been unregistered, <code>false</code> is returned.
   * <li>The context bundle's use count for this service is decremented by
   * one.
   * <li>If the context bundle's use count for the service is currently zero
   * and the service was registered with a <code>ServiceFactory</code> object,
   * the ServiceFactory#UngetService
   * method is called to release the service object for the context bundle.
   * <li><code>true</code> is returned.
   * </ol>
   *
   * @param reference A reference to the service to be released.
   * @return <code>false</code> if the context bundle's use count for the
   *         service is zero or if the service has been unregistered;
   *         <code>true</code> otherwise.
   * @throws IllegalStateException If this BundleContext is no longer valid.
   * @throws InvalidArgumentException If the specified
   *         {@code ServiceReference} was not created by the same framework
   *         instance as this {@code BundleContext}.
   * @see #getService
   * @see ServiceFactory
   */
  virtual bool ungetService(const ServiceReferenceBase& reference) const = 0;

  virtual void addServiceListener(const ServiceListener& delegate,
                                  const std::string& filter = std::string()) = 0;
  virtual void removeServiceListener(const ServiceListener& delegate) = 0;

  virtual void addBundleListener(const BundleListener& delegate) = 0;
  virtual void removeBundleListener(const BundleListener& delegate) = 0;

  /**
   * Adds the specified {@code FrameworkListener} object to the context
   * bundle's list of listeners if not already present. FrameworkListeners are
   * notified of general Framework events.
   *
   * <p>
   * If the context bundle's list of listeners already contains a listener
   * {@code l} such that {@code (l==listener)}, this method does nothing.
   *
   * @param listener The {@code FrameworkListener} object to be added.
   * @throws IllegalStateException If this BundleContext is no longer valid.
   * @see FrameworkEvent
   * @see FrameworkListener
   */
  virtual void addFrameworkListener(const FrameworkListener& delegate) = 0;

  /**
   * Removes the specified {@code FrameworkListener} object from the context
   * bundle's list of listeners.
   *
   * <p>
   * If {@code listener} is not contained in the context bundle's list of
   * listeners, this method does nothing.
   *
   * @param listener The {@code FrameworkListener} object to be removed.
   * @throws IllegalStateException If this BundleContext is no longer valid.
   */
  virtual void removeFrameworkListener(const FrameworkListener& delegate) = 0;

  /**
   * Adds the specified <code>callback</code> with the
   * specified <code>filter</code> to the context modules's list of listeners.
   * See LDAPFilter for a description of the filter syntax. Listeners
   * are notified when a service has a lifecycle state change.
   *
   * <p>
   * You must take care to remove registered listeners befor the <code>receiver</code>
   * object is destroyed. However, the Framework takes care
   * of removing all listeners registered by this context bundle's classes
   * after the bundle is unloaded.
   *
   * <p>
   * If the context bundle's list of listeners already contains a pair <code>(r,c)</code>
   * of <code>receiver</code> and <code>callback</code> such that
   * <code>(r == receiver && c == callback)</code>, then this
   * method replaces that callback's filter (which may be empty)
   * with the specified one (which may be empty).
   *
   * <p>
   * The callback is called if the filter criteria is met. To filter based
   * upon the class of the service, the filter should reference the
   * Constants#OBJECTCLASS() property. If <code>filter</code> is
   * empty, all services are considered to match the filter.
   *
   * <p>
   * When using a <code>filter</code>, it is possible that the
   * <code>ServiceEvent</code>s for the complete lifecycle of a service
   * will not be delivered to the callback. For example, if the
   * <code>filter</code> only matches when the property <code>x</code> has
   * the value <code>1</code>, the callback will not be called if the
   * service is registered with the property <code>x</code> not set to the
   * value <code>1</code>. Subsequently, when the service is modified
   * setting property <code>x</code> to the value <code>1</code>, the
   * filter will match and the callback will be called with a
   * <code>ServiceEvent</code> of type <code>MODIFIED</code>. Thus, the
   * callback will not be called with a <code>ServiceEvent</code> of type
   * <code>REGISTERED</code>.
   *
   * @tparam The type of the receiver (containing the member function to be called)
   * @param receiver The object to connect to.
   * @param callback The member function pointer to call.
   * @param filter The filter criteria.
   * @throws InvalidSyntaxException If <code>filter</code> contains an
   *         invalid filter string that cannot be parsed.
   * @throws IllegalStateException If this BundleContext is no
   *         longer valid.
   * @see ServiceEvent
   * @see RemoveServiceListener()
   */
  template<class R>
  void addServiceListener(R* receiver, void(R::*callback)(const ServiceEvent),
                          const std::string& filter = std::string());

  /**
   * Removes the specified <code>callback</code> from the context bundle's
   * list of listeners.
   *
   * <p>
   * If the <code>(receiver,callback)</code> pair is not contained in this
   * context bundle's list of listeners, this method does nothing.
   *
   * @tparam The type of the receiver (containing the member function to be removed)
   * @param receiver The object from which to disconnect.
   * @param callback The member function pointer to remove.
   * @throws IllegalStateException If this BundleContext is no
   *         longer valid.
   * @see AddServiceListener()
   */
  template<class R>
  void removeServiceListener(R* receiver, void(R::*callback)(const ServiceEvent));

  /**
   * Adds the specified <code>callback</code> to the context modules's list
   * of listeners. Listeners are notified when a bundle has a lifecycle
   * state change.
   *
   * <p>
   * If the context bundle's list of listeners already contains a pair <code>(r,c)</code>
   * of <code>receiver</code> and <code>callback</code> such that
   * <code>(r == receiver && c == callback)</code>, then this method does nothing.
   *
   * @tparam The type of the receiver (containing the member function to be called)
   * @param receiver The object to connect to.
   * @param callback The member function pointer to call.
   * @throws IllegalStateException If this BundleContext is no
   *         longer valid.
   * @see BundleEvent
   */
  template<class R>
  void addBundleListener(R* receiver, void(R::*callback)(const BundleEvent));

  /**
   * Removes the specified <code>callback</code> from the context bundle's
   * list of listeners.
   *
   * <p>
   * If the <code>(receiver,callback)</code> pair is not contained in this
   * context bundle's list of listeners, this method does nothing.
   *
   * @tparam The type of the receiver (containing the member function to be removed)
   * @param receiver The object from which to disconnect.
   * @param callback The member function pointer to remove.
   * @throws IllegalStateException If this BundleContext is no
   *         longer valid.
   * @see AddBundleListener()
   */
  template<class R>
  void removeBundleListener(R* receiver, void(R::*callback)(const BundleEvent));

  template<class R>
  void addFrameworkListener(R* receiver, void(R::*callback)(const FrameworkEvent));

  template<class R>
  void removeFrameworkListener(R* receiver, void(R::*callback)(const FrameworkEvent));

  /**
   * Creates a string containing the location for a file in the persistent storage area
   * provided for the bundle by the Framework. This method will return
   * an empty string if the platform does not have file system support.
   *
   * <p>
   * A string containing the location for the base directory of the persistent storage
   * area provided for the context bundle by the Framework can be obtained by
   * calling this method with an empty string as {@code filename}.
   *
   * @param filename A relative name to the file to be accessed.
   * @return A {@code std::string} object that represents the requested file or
   *       an empty string if the platform does not have file system support.
   * @throws IllegalStateException If this BundleContext is no longer valid.
   */
  virtual std::string getDataFile(const std::string& filename) const = 0;

  /**
   * Creates a {@code Filter} object. This {@code Filter} object may be used
   * to match a {@code ServiceReference} object or a {@code Dictionary}
   * object.
   *
   * <p>
   * If the filter cannot be parsed, an {@link InvalidSyntaxException} will be
   * thrown with a human readable message where the filter became unparsable.
   *
   * @param filter The filter string.
   * @return A {@code Filter} object encapsulating the filter string.
   * @throws InvalidSyntaxException If {@code filter} contains an invalid
   *         filter string that cannot be parsed.
   * @throws NullPointerException If {@code filter} is null.
   * @throws IllegalStateException If this BundleContext is no longer valid.
   * @see "Framework specification for a description of the filter string syntax."
   * @see FrameworkUtil#createFilter(String)
   */
  virtual Filter createFilter(const std::string& filter) const = 0;

  /**
   * Returns the bundle with the specified location.
   *
   * @param location The location of the bundle to retrieve.
   * @return A {@code Bundle} object or {@code null} if the location does not
   *         match any installed bundle.
   */
  virtual Bundle* getBundle(const std::string& location) const = 0;

protected:

  BundleContext();

private:

  // purposely not implemented
  BundleContext(const BundleContext&);
  BundleContext& operator=(const BundleContext&);

};

} // end namespace osgi

#include "BundleContext.txx"

#endif // OSGI_IBUNDLECONTEXT_H_
