cmake_minimum_required(VERSION 3.16)

project(base LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/source-charset:utf-8)
endif()

set(SOURCES 
    box_yxbase.h box_yxbase.cpp
)

add_library(base STATIC ${SOURCES})
target_include_directories(base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})