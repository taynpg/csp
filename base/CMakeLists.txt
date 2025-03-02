cmake_minimum_required(VERSION 3.16)

project(base LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED USE_UTF8_ALL)
if (MSVC)
    add_compile_options(/source-charset:utf-8)
endif()
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "base Add MinGW Param.")
    add_compile_options(-finput-charset=utf-8)
    add_compile_options(-fexec-charset=gbk)
endif()
endif()

set(SOURCES base.h base.cxx)

add_library(base STATIC ${SOURCES})
target_include_directories(base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})