cmake_minimum_required(VERSION 3.16)
project(sxtwl)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)

if (MSVC)
    add_compile_options(/source-charset:utf-8)
endif()
file(GLOB SRC_FILES  "*.cpp" )

if(UNIX)
    add_compile_options(-fPIC)
elseif(MSVC)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
endif()

add_library(sxtwl STATIC ${SRC_FILES})
target_include_directories(sxtwl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})