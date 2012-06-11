cmake_minimum_required(VERSION 2.8)

project(org_nativeosgi_greetingservice_interface)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/public/include)

set(src_files
  private/src/GreetingServiceBindings.cpp
)

set(public_header
  public/include/greeting_service.h
  public/include/IGreetingService.h
)

add_library(${PROJECT_NAME} SHARED ${src_files} ${public_header})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-rdynamic")

