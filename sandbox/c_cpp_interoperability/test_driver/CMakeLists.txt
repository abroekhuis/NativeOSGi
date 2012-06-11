
configure_file(test_driver_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/test_driver_config.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(test_driver main.cpp)
target_link_libraries(test_driver nativeosgi_cpp_framework dl)
