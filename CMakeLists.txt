#
# Copyright 2023 The titan-search Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
####################################################################################
# generate by carbin package manager tools for titan-search
# https://github.com/gottingen/carbin
####################################################################################
cmake_minimum_required(VERSION 3.24)

project(collie CXX)

option(${PROJECT_NAME}_IS_TOP_PROJECT "collie project is the top level project or not" ON)

get_property(not_top DIRECTORY PROPERTY PARENT_DIRECTORY)
if (not_top)
    set(${PROJECT_NAME}_IS_TOP_PROJECT OFF)
endif ()
message(STATUS ${PROJECT_NAME}_IS_TOP_PROJECT)

set(PROJECT_DESCRIPTION "Hercules is an ahead of time compiler for a subset of the Python language framework.")
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 2)
set(PROJECT_VERSION_PATCH 38)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

set(${PROJECT_NAME}_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(${PROJECT_NAME}_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(${PROJECT_NAME}_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(${PROJECT_NAME}_VERSION ${PROJECT_VERSION})


set(CARBIN_VERSION 0.5.0)
set(CARBIN_CMAKE_VERSION 0.5.0)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/carbin_cmake)
##################################################################
# add this for ${PROJECT_NAME} 's cmake include
##################################################################
list(APPEND CMAKE_MODULE_PATH ${${PROJECT_NAME}_SOURCE_DIR}/cmake)
#################################################################
#
# modify the CARBIN_DEPS_PREFIX to you prefix if needed.
# for example when you install you deps to "/opt/handsome/guy
# modify as set(CARBIN_DEPS_PREFIX /opt/handsome/guy)
# this should be PROJECT_SOURCE_DIR, only install libraries
# deps to top level project
#################################################################
set(CARBIN_DEPS_PREFIX ${PROJECT_SOURCE_DIR}/carbin)
#######################################################################
# warning
# in carbin_cmake directory, caution to modify files in that dir
######################################################################
include(carbin_module)

##################################################################
#
# your cmake directory ${PROJECT_SOURCE_DIR}/cmake, do things here
#################################################################
include(collie_deps)
include(collie_cxx_config)
include(collie_test)

configure_file(${PROJECT_SOURCE_DIR}/conda/meta.yaml.in ${PROJECT_SOURCE_DIR}/conda/meta.yaml @ONLY)

add_subdirectory(collie)
####################################################################
# belows are auto, edit it be cation
####################################################################
if (CARBIN_BUILD_TEST)
    add_subdirectory(tests)
endif ()

if (CARBIN_BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif ()

if (CARBIN_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

##############################################
# header installing
#############################################
install(DIRECTORY ${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING
        PATTERN "*.inc"
        PATTERN "*.h"
        PATTERN "*.hpp"
        )

# Layout. This works for all platforms:
#   * <prefix>/@CMAKE_INSTALL_LIBDIR@/cmake/<PROJECT-NAME>
#   * <prefix>/@CMAKE_INSTALL_LIBDIR@*/
#   * <prefix>/include/
set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Configuration
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

# Include module with fuction 'write_basic_package_version_file'
include(CMakePackageConfigHelpers)

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
# Use:
#   * PROJECT_VERSION
write_basic_package_version_file(
        "${version_config}" COMPATIBILITY SameMajorVersion
)

# Configure '<PROJECT-NAME>Config.cmake'
# Use variables:
#   * TARGETS_EXPORT_NAME
#   * PROJECT_NAME
configure_package_config_file(
        "cmake/collie_config.cmake.in"
        "${project_config}"
        INSTALL_DESTINATION "${config_install_dir}"
)

#Targets:
#  * <prefix>/${CMAKE_INSTALL_LIBDIR}/libbar.a
#  * <prefix>/${CMAKE_INSTALL_LIBDIR}/libbaz.a
#  * header location after install: <prefix>/include/foo/Bar.hpp
#  * headers can be included by C++ code `#include <foo/Bar.hpp>`


# Config
#   * <prefix>/${CMAKE_INSTALL_LIBDIR}/cmake/Foo/FooConfig.cmake
#   * <prefix>/${CMAKE_INSTALL_LIBDIR}/cmake/Foo/FooConfigVersion.cmake
install(
        FILES
        "${project_config}" "${version_config}"
        DESTINATION
        "${config_install_dir}"
)

# Config
#   * <prefix>/${CMAKE_INSTALL_LIBDIR}/cmake/Foo/FooTargets.cmake

install(
        EXPORT
        "${TARGETS_EXPORT_NAME}"
        NAMESPACE
        "${namespace}"
        DESTINATION
        "${config_install_dir}"
)
configure_file(${PROJECT_SOURCE_DIR}/carbin_cmake/package/pkg_dump_template.pc.in
        ${PROJECT_BINARY_DIR}/package/${PROJECT_NAME}.pc)

install(FILES ${PROJECT_BINARY_DIR}/package/${PROJECT_NAME}.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
include(collie_cpack_config)