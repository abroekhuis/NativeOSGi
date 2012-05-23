cmake_minimum_required(VERSION 2.8)

project(org_nativeosgi_cppgreetingserviceconsumer)

set(src_files
  private/src/greeting_service_consumer.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/greeting_interface_bundle/public/include)

add_library(${PROJECT_NAME} SHARED ${src_files})
target_link_libraries(${PROJECT_NAME}
                      org_nativeosgi_greetingservice_interface # A C++ bundle needs to link to the interface bundle
                      cpp_sb_registry
                      )

