
project(nativeosgicpp)

set(_src_files
  Any.cpp
  BundleContext.cpp
  BundleException.cpp
  Constants.cpp
  Exception.cpp
  InvalidSyntaxException.cpp
  ServiceException.cpp
  ServiceReferenceBase.cpp
  ServiceRegistrationBase.cpp
)

set(_h_files
  Any.h
  AtomicInt.h
  BundleContext.h
  BundleContext.txx
  BundleException.h
  Constants.h
  Exception.h
  Export.h
  InvalidSyntaxException.h
  ServiceException.h
  ServiceInterface.h
  ServiceProperties.h
  ServiceReference.h
  ServiceReferenceBase.h
  ServiceRegistration.h
  ServiceRegistrationBase.h
  SharedData.h
  Threads.h
)

set(_configured_h_files
  Config.h
)

set(OSGI_ENABLE_THREADING_SUPPORT 1)

set(_all_files )
foreach(_file ${_src_files})
  list(APPEND _all_files src/${_file})
endforeach()
foreach(_file ${_h_files})
  list(APPEND _all_files include/osgi/${_file})
endforeach()
foreach(_file ${_configured_h_files})
  configure_file(include/osgi/${_file}.in ${CMAKE_CURRENT_BINARY_DIR}/include/osgi/${_file})
  list(APPEND _all_files ${CMAKE_CURRENT_BINARY_DIR}/include/osgi/${_file})
endforeach()

include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
include_directories(../common/include)
include_directories(include)

add_library(${PROJECT_NAME} SHARED ${_all_files})
