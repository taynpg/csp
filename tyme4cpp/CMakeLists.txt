cmake_minimum_required(VERSION 3.16)

project(tyme4cpp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
tyme.cpp
tyme.h
util.h
util.cpp
)
add_library(tyme4cpp STATIC ${SOURCES})
target_include_directories(tyme4cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
