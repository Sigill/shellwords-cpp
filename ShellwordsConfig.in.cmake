get_filename_component(PACKAGE_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

if(NOT TARGET shellwords)
  include("${PACKAGE_CMAKE_DIR}/@SHELLWORDS_EXPORT@.cmake")
endif()
