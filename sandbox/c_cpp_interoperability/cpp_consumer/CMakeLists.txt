cmake_minimum_required(VERSION 2.8)

project(org_nativeosgi_cppserviceconsumer)

set(src_files
  private/src/service_consumer.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/greeting_interface_bundle/public/include)
include_directories(${CMAKE_SOURCE_DIR}/cpp_complexservice_provider/public/include)
include_directories(${CMAKE_SOURCE_DIR}/cpp_cppserviceonly_provider/public/include)

add_library(${PROJECT_NAME} SHARED ${src_files})
target_link_libraries(${PROJECT_NAME}
                      org_nativeosgi_greetingservice_interface # A C++ bundle needs to link to the interface bundle
                      org_nativeosgi_cppcomplexserviceprovider
                      org_nativeosgi_cppserviceonlyprovider
                      cpp_sb_registry
                      )

