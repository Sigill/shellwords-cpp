get_filename_component(PACKAGE_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(SHELLWORDS_INCLUDE_DIRS "@CONF_INCLUDE_DIRS@")

if(NOT TARGET shellwords)
  include("${PACKAGE_CMAKE_DIR}/@SHELLWORDS_EXPORT@.cmake")
endif()
