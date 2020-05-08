cmake_minimum_required(VERSION 2.8.12)
project(shellwords-cpp)

set(SHELLWORDSCPP_VERSION "0.2.0")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(shellwords shellwords.cxx)
target_include_directories(shellwords
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
           $<INSTALL_INTERFACE:include>)

# Define public headers to get them automatically installed.
set_target_properties(shellwords PROPERTIES
    PUBLIC_HEADER shellwords.hxx)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(SHELLWORDS_EXPORT ShellwordsTargets)

# Install the lib and its headers. Flag it for export.
install(TARGETS shellwords
    EXPORT ${SHELLWORDS_EXPORT}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/shellwords
    )

# Create the export file for the build tree.
export(TARGETS shellwords FILE "${PROJECT_BINARY_DIR}/${SHELLWORDS_EXPORT}.cmake")
# Create the export file for the install tree.
install(EXPORT ${SHELLWORDS_EXPORT}
    DESTINATION "${CMAKE_INSTALL_PREFIX}")

# The config files could be generated using a macro (see [1]), but it seems
# to be more complex than to do it manually (see [2]).
# [1] https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html
# [2] https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/How-to-create-a-ProjectConfig.cmake-file

# Create the config file for the build tree.
set(CONF_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
configure_file(ShellwordsConfig.in.cmake
    "${PROJECT_BINARY_DIR}/ShellwordsConfig.cmake" @ONLY)

# Create the config file for the install tree.
set(CONF_INCLUDE_DIRS "\${PACKAGE_CMAKE_DIR}/${CMAKE_INSTALL_INCLUDEDIR}")
configure_file(ShellwordsConfig.in.cmake
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ShellwordsConfig.cmake" @ONLY)

# Create the package version file.
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ShellwordsConfigVersion.cmake"
    VERSION "${SHELLWORDSCPP_VERSION}"
    COMPATIBILITY SameMajorVersion)

# Install both the config and version file.
install(FILES
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/ShellwordsConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ShellwordsConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_PREFIX}")

include(CTest)

if(BUILD_TESTING)
    find_package(GTest REQUIRED NO_MODULE)
    enable_testing()

    add_executable(unit-tests tests.cxx)
    target_link_libraries(unit-tests PRIVATE GTest::gmock_main shellwords)
    add_test(unit-tests unit-tests)
endif()
