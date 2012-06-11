
#-----------------------------------------------------------------------------
# Configure files
#-----------------------------------------------------------------------------

set(VISIBILITY_AVAILABLE 0)
set(visibility_test_flag "")
osgiFunctionCheckCompilerFlags("-fvisibility=hidden" visibility_test_flag)
if(visibility_test_flag)
  # The compiler understands -fvisiblity=hidden (probably gcc >= 4 or Clang)
  set(VISIBILITY_AVAILABLE 1)
endif()
configure_file(util/include/osgi/ExportMacros.h.in ${CMAKE_CURRENT_BINARY_DIR}/util/include/osgi/ExportMacros.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/util/include)

#-----------------------------------------------------------------------------
# Add sub-directories
#-----------------------------------------------------------------------------

if(BUILD_C++_API)
  add_subdirectory(api/cpp)
endif()


