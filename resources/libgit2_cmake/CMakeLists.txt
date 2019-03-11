# Copyright 2019, alex at staticlibs.net
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required ( VERSION 2.8.12 )
project ( git2 C )
include ( ${CMAKE_CURRENT_LIST_DIR}/../macros.cmake )

if ( NOT DEFINED ${PROJECT_NAME}_LOOKASIDE_DIR )
    set ( ${PROJECT_NAME}_LOOKASIDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../lookaside_libgit2 CACHE INTERNAL "" )
endif ( )

# dependencies
set ( ${PROJECT_NAME}_DEPS openssl libssh2 )
external_libgit2_pkg_check_modules ( ${PROJECT_NAME}_DEPS_PC REQUIRED ${PROJECT_NAME}_DEPS )

# check deplibs cache
if ( STATICLIB_USE_DEPLIBS_CACHE )
    set ( ${PROJECT_NAME}_CACHED_LIB_PATH ${STATICLIB_DEPLIBS_CACHE_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX} )
    if ( EXISTS ${${PROJECT_NAME}_CACHED_LIB_PATH} )
        add_library( ${PROJECT_NAME} STATIC IMPORTED GLOBAL )
        set_target_properties( ${PROJECT_NAME} PROPERTIES IMPORTED_LOCATION ${${PROJECT_NAME}_CACHED_LIB_PATH} )
        message ( STATUS "Using cached library: [${${PROJECT_NAME}_CACHED_LIB_PATH}]" )
        return ( )
    endif ( )
endif ( )

# sources
set ( ${PROJECT_NAME}_INCLUDES_PLATFORM )
file ( GLOB ${PROJECT_NAME}_SRC
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/*.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/streams/*.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/transports/*.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/xdiff/*.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/hash/hash_collisiondetect.c
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/hash/sha1dc/*
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/deps/http-parser/*.c )
if ( external_libgit2_TOOLCHAIN MATCHES "windows_[^_]+_[^_]+" )
    file ( GLOB ${PROJECT_NAME}_SRC_WIN32
            ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/win32/*.c
            ${${PROJECT_NAME}_LOOKASIDE_DIR}/deps/regex/regex.c )
    list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_SRC_WIN32} )
    list ( APPEND ${PROJECT_NAME}_INCLUDES_PLATFORM ${${PROJECT_NAME}_LOOKASIDE_DIR}/deps/regex/ )
else ( )
    file ( GLOB ${PROJECT_NAME}_SRC_UNIX
            ${${PROJECT_NAME}_LOOKASIDE_DIR}/src/unix/*.c )
    list ( APPEND ${PROJECT_NAME}_SRC ${${PROJECT_NAME}_SRC_UNIX} )
endif ( )
source_group ( "src" FILES ${${PROJECT_NAME}_SRC} )

# library
add_library ( ${PROJECT_NAME} STATIC 
        ${${PROJECT_NAME}_SRC} )
target_include_directories ( ${PROJECT_NAME} BEFORE PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include_${external_libgit2_TOOLCHAIN}
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/include
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/src
        ${${PROJECT_NAME}_LOOKASIDE_DIR}/deps/http-parser
        ${${PROJECT_NAME}_INCLUDES_PLATFORM}
        ${${PROJECT_NAME}_DEPS_PC_INCLUDE_DIRS} )
target_compile_definitions ( ${PROJECT_NAME} PRIVATE )
