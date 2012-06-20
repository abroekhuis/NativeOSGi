
option(DOXYGEN_SKIP_DOT "Use dot program for generating graphical class diagrams with doxygen, if available" OFF)
mark_as_advanced(DOXYGEN_SKIP_DOT)

find_package(Doxygen)

if(DOXYGEN_FOUND)

  if(BUILD_C++_API)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE PATH "Output directory for doxygen generated documentation." )
    set(DOXYGEN_TAGFILE_NAME ${DOXYGEN_OUTPUT_DIR}/NativeOSGi_C++.tag CACHE INTERNAL "Doxygen tag file")

    configure_file(cpp_api_doxygen.conf.in
                   ${CMAKE_CURRENT_BINARY_DIR}/cpp_api_doxygen.conf)

    add_custom_target(doc_cpp ${DOXYGEN} ${CMAKE_CURRENT_BINARY_DIR}/cpp_api_doxygen.conf
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                     )
  endif()

endif(DOXYGEN_FOUND)

