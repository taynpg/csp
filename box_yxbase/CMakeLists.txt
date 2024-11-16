cmake_minimum_required(VERSION 3.16)

project(box_yxbase)
add_library(box_yxbase STATIC "box_yxbase.h" "box_yxbase.cpp")
target_include_directories(box_yxbase PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
