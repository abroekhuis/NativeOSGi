include_directories(${CMAKE_BINARY_DIR}/src/util/include)
include_directories(${CMAKE_SOURCE_DIR}/src/util/include)

include_directories(${CMAKE_BINARY_DIR}/src/api/cpp/include)
include_directories(${CMAKE_SOURCE_DIR}/src/api/cpp/include)

add_subdirectory(cpp_framework)

include_directories(${CMAKE_SOURCE_DIR}/sandbox/api/c/include)
add_subdirectory(api_bindings/c)
add_subdirectory(c_cpp_interoperability)

add_subdirectory(bfd)
