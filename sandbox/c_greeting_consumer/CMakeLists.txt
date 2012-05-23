cmake_minimum_required(VERSION 2.8)

project(org_nativeosgi_cgreetingserviceconsumer)

set(src_files
  private/src/greeting_service_consumer.c
)

include_directories(${CMAKE_SOURCE_DIR}/greeting_interface_bundle/public/include)

add_library(${PROJECT_NAME} SHARED ${src_files})
target_link_libraries(${PROJECT_NAME} cpp_sb_registry)

