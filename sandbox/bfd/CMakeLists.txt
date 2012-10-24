add_executable(bfd_package_patcher bfd_package_patcher.c)

add_library(org_package_a SHARED org_package_a.c)
add_library(org_package_b SHARED org_package_b.c)

target_link_libraries(org_package_a org_package_b)
target_link_libraries(bfd_package_patcher bfd)
