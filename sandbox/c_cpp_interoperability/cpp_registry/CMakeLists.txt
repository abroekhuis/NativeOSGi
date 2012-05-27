cmake_minimum_required(VERSION 2.8)

project(cpp_sb_registry)

set(src_files
  sb_registry.cpp
  sb_osgi_interface.h
)

add_library(${PROJECT_NAME} SHARED ${src_files})
target_link_libraries(${PROJECT_NAME} dl)

