
project(sb_nativeosgi_c_binding)

set(srcs
  bundle_context_impl.cpp
)

# This is only needed to get a BundleContext instance
include_directories(${CMAKE_SOURCE_DIR}/sandbox/cpp_framework)

add_library(${PROJECT_NAME} SHARED ${srcs})

# We link to our sandbox implementation, but this could be
# any C++ framework implementation.
target_link_libraries(${PROJECT_NAME} nativeosgi_cpp_framework)
