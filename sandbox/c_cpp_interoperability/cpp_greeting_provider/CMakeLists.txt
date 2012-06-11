cmake_minimum_required(VERSION 2.8)

project(org_nativeosgi_cppgreetingserviceprovider)

set(src_files
  private/src/CppGreeter.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../greeting_interface_bundle/public/include)

add_library(${PROJECT_NAME} SHARED ${src_files})
target_link_libraries(${PROJECT_NAME}
                      org_nativeosgi_greetingservice_interface
                      nativeosgi_cpp_framework
                      )

