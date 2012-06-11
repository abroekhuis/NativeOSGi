cmake_minimum_required(VERSION 2.8)

project(nativeosgi_cpp_framework)

set(src_files
  BundleContextImpl.cpp
  ServiceHandle.h
  ServiceReferenceImpl.cpp
  ServiceRegistrationImpl.cpp
  ServiceRegistry.cpp
)

add_library(${PROJECT_NAME} SHARED ${src_files})
target_link_libraries(${PROJECT_NAME} nativeosgicpp dl)

