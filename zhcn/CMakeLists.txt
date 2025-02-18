cmake_minimum_required(VERSION 3.16)

project(zhcn LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/source-charset:utf-8)
endif()

set(SOURCES zh_lang.h zh_lang.cxx)
add_library(zhcn STATIC ${SOURCES})
target_include_directories(zhcn PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})