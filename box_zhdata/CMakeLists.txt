cmake_minimum_required(VERSION 3.16)

project(box_zhdata)
if (MSVC)
    add_compile_options(/utf-8)
endif()
add_library(box_zhdata STATIC "zh_lang.h" "zh_lang.cpp")
target_include_directories(box_zhdata PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})