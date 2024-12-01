cmake_minimum_required(VERSION 3.16)

project(qimen LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/source-charset:utf-8)
endif()

set(SOURCES 
    qm_base.h qm_base.cpp qm_v1.cpp qm_v1.h
    qm_v2.h qm_v2.cpp qm_v3.h qm_v3.cpp
    qm_v4.h qm_v4.cpp
)

add_library(qimen STATIC ${SOURCES})
target_link_libraries(qimen PRIVATE calendar)
target_include_directories(qimen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})