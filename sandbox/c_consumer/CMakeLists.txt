cmake_minimum_required(VERSION 2.8)

project(org_nativeosgi_cserviceconsumer)

set(src_files
  private/src/service_consumer.c
)

include_directories(${CMAKE_SOURCE_DIR}/greeting_interface_bundle/public/include)
include_directories(${CMAKE_SOURCE_DIR}/cpp_complexservice_provider/public/include)

add_library(${PROJECT_NAME} SHARED ${src_files})
target_link_libraries(${PROJECT_NAME} cpp_sb_registry)

