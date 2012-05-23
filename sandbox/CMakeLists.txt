cmake_minimum_required(VERSION 2.8)

project(nativeosgi_sandbox)

add_subdirectory(cpp_registry)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cpp_registry)

add_subdirectory(greeting_interface_bundle)

add_subdirectory(cpp_greeting_provider)
add_subdirectory(c_greeting_provider)
add_subdirectory(c_greeting_consumer)
add_subdirectory(cpp_greeting_consumer)

add_subdirectory(test_driver)
