cmake_minimum_required(VERSION 2.8)

project(org_nativeosgi_cgreetingserviceprovider)

set(src_files
  private/src/greeting_impl.c
)

set(private_header
  private/include/greeting_impl.h
)

include_directories(${CMAKE_SOURCE_DIR}/greeting_interface_bundle/public/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/private/include)

add_library(${PROJECT_NAME} SHARED ${src_files} ${private_header})
target_link_libraries(${PROJECT_NAME} cpp_sb_registry)
