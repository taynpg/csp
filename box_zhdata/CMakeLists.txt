cmake_minimum_required(VERSION 3.8)

project(box_zhdata)
add_library(box_zhdata SHARED "zh_lang.h" "zh_lang.cpp")
target_compile_definitions(box_zhdata PRIVATE CPP_ZHDATA_LIB)
