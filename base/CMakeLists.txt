cmake_minimum_required(VERSION 3.16)

project(base LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES base.h base.cpp)
add_library(base STATIC ${SOURCES})
target_include_directories(base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})