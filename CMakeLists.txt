cmake_minimum_required(VERSION 3.0)
project(debugstream)

set(MY_LIB_NAME ${PROJECT_NAME})
# set(MY_LIB_NAME "debugstream")
message("[info]: set lib name: ${MY_LIB_NAME}")

include(cmake/help.cmake)

# define library version (update: apparently you can also do it in project()!)
set(MY_LIBRARY_VERSION_MAJOR 3 CACHE STRING "major version" FORCE)
set(MY_LIBRARY_VERSION_MINOR 1 CACHE STRING "minor version" FORCE)
set(MY_LIBRARY_VERSION_PATCH 5 CACHE STRING "patch version" FORCE)
set(MY_LIBRARY_VERSION ${MY_LIBRARY_VERSION_MAJOR}.${MY_LIBRARY_VERSION_MINOR}.${MY_LIBRARY_VERSION_PATCH} CACHE STRING "version" FORCE)
PrintVariable(MY_LIBRARY_VERSION)

option(CMAKE_BUILD_TYPE Release) # Debug Release RelWithDebInfo
# set a debug postfix
set(CMAKE_DEBUG_POSTFIX "-dbg")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# installation destinations
# must set before add_subdirectory
if(UNIX OR VXWORKS)
    include(GNUInstallDirs)
    set(MY_LIBRARY_INC_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
    set(MY_LIBRARY_RUNTIME_INSTALL_DIR "${CMAKE_INSTALL_BINDIR}")
    set(MY_LIBRARY_LIBRARY_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
    set(MY_LIBRARY_ARCHIVE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
    set(MY_LIBRARY_FRAMEWORK_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
    set(MY_LIBRARY_CMAKE_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/${MY_LIB_NAME}/cmake")
    set(MY_LIBRARY_ADDITIONAL_FILES_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/${MY_LIB_NAME}")
elseif(WIN32)
    set(MY_LIBRARY_INC_INSTALL_DIR "include")
    set(MY_LIBRARY_RUNTIME_INSTALL_DIR   "bin")
    set(MY_LIBRARY_LIBRARY_INSTALL_DIR   "bin")
    set(MY_LIBRARY_ARCHIVE_INSTALL_DIR   "lib")
    set(MY_LIBRARY_FRAMEWORK_INSTALL_DIR "bin")
    set(MY_LIBRARY_CMAKE_CONFIG_INSTALL_DIR "share/${MY_LIB_NAME}/cmake")
    set(MY_LIBRARY_ADDITIONAL_FILES_INSTALL_DIR "share/${MY_LIB_NAME}")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
else()
    message(FATAL_ERROR "Could not set install folders for this platform!")
endif()


# set default build control
option(BUILD_EXAMPLES "whether or not to build the examples" ON)
option(BUILD_TESTS "whether or not to build the tests" ON)
option(BUILD_TOOLS "whether or not to build the tests" ON)
# add subdiretories
add_subdirectory(src)
if(BUILD_EXAMPLES)
    add_subdirectory(example)
endif()
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
if(BUILD_TOOLS)
    enable_testing()
    add_subdirectory(tool)
endif()

# install readme and license
install(FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
  "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
  DESTINATION ${MY_LIBRARY_ADDITIONAL_FILES_INSTALL_DIR}
)

# install generate cmake files
install(EXPORT ${MY_LIB_NAME}_targets DESTINATION ${MY_LIBRARY_CMAKE_CONFIG_INSTALL_DIR}
                                       FILE ${MY_LIB_NAME}-config.cmake)
