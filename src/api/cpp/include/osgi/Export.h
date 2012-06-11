#pragma once
#ifndef OSGI_NATIVEOSGICPPEXPORT_H_
#define OSGI_NATIVEOSGICPPEXPORT_H_

#include <osgi/ExportMacros.h>

#ifdef nativeosgicpp_EXPORTS
  #define OSGI_CPP_EXPORT OSGI_ABI_EXPORT
#else
  #define OSGI_CPP_EXPORT OSGI_ABI_IMPORT
#endif

#endif // OSGI_NATIVEOSGICPPEXPORT_H_
