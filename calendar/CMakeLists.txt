cmake_minimum_required(VERSION 3.16)

project(calendar LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/source-charset:utf-8)
endif()

set(SOURCES 
    calendar_v1.h calendar_v1.cpp calendar_v1_data.h calendar_v1_data.cpp
    calendar_v2.h calendar_v2.cpp
    calendar_base.h calendar_base.cpp
)
add_library(calendar STATIC ${SOURCES})
target_link_libraries(calendar PRIVATE sxtwl)
target_include_directories(calendar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})