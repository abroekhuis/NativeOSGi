#pragma once
#ifndef OSGI_SERVICEINTERFACE_H_
#define OSGI_SERVICEINTERFACE_H_

#include <osgi/MacroUtils.h>

/**
 * \cond internal
 *
 * Returns a unique id for a given type.
 *
 * This template method is specialized in the macro
 * OSGI_DECLARE_SERVICE_INTERFACE to return a unique id.
 */
template<class T>
inline const char* osgi_service_interface_id()
{
  return 0;
}
/** \endcond */

/**
 * \ingroup cpp_api
 *
 * This macro associates the given \e Identifier (a string literal) to the interface class called IFace.
 * The Identifier must be unique. For example:
 *
 * \code
 * #include <osgi/MacroUtils.h>
 *
 * #define ICameraDevice_ID com_mycompany_ICameraDevice_1_0
 * #define ICameraDevice_NAME OSGI_XSTR(ICameraDevice_ID)
 *
 * struct ICameraDevice { ... };
 *
 * OSGI_DECLARE_SERVICE_INTERFACE(ICameraDevice, ICameraDevice_NAME)
 * \endcode
 *
 * This macro is normally used right after the class definition for IFace, in a header file.
 *
 * If you want to use OSGI_DECLARE_SERVICE_INTERFACE with interface classes declared in a
 * namespace then you have to make sure the OSGI_DECLARE_SERVICE_INTERFACE is not inside a
 * namespace though. For example:
 *
 * \code
 * #include <osgi/MacroUtils.h>
 *
 * #define MyInterface_ID com_mycompany_MyInterface_1_0
 * #define MyInterface_NAME OSGI_XSTR(MyInterface_ID)
 *
 * namespace Foo
 * {
 *   struct MyInterface { ... };
 * }
 *
 * OSGI_DECLARE_SERVICE_INTERFACE(Foo::MyInterface, MyInterface_NAME)
 * \endcode
 */
#define OSGI_DECLARE_SERVICE_INTERFACE(IFace, Identifier) \
  template<> \
  inline const char* osgi_service_interface_id<IFace*>() \
  { \
    return Identifier; \
  }

#endif // OSGI_SERVICEINTERFACE_H_
