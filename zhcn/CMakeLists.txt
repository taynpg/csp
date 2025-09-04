cmake_minimum_required(VERSION 3.16)

project(zhcn LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES zh_lang.h zh_lang.cpp)
add_library(zhcn STATIC ${SOURCES})
target_include_directories(zhcn PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
