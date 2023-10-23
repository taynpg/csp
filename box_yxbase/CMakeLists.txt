cmake_minimum_required(VERSION 3.8)

project(box_yxbase)
add_library(box_yxbase SHARED "box_yxbase.h" "box_yxbase.cpp")
target_compile_definitions(box_yxbase PRIVATE CPP_YX_BASE_LIB)
