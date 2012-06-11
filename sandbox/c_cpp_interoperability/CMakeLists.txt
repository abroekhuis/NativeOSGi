cmake_minimum_required(VERSION 2.8)

project(nativeosgi_sandbox)

include_directories(${CMAKE_SOURCE_DIR}/sandbox/cpp_framework)

add_subdirectory(greeting_interface_bundle)

add_subdirectory(cpp_greeting_provider)
add_subdirectory(cpp_complexservice_provider)
add_subdirectory(cpp_cppserviceonly_provider)
add_subdirectory(c_greeting_provider)

add_subdirectory(c_consumer)
add_subdirectory(cpp_consumer)

add_subdirectory(test_driver)
