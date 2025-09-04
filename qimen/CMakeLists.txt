cmake_minimum_required(VERSION 3.16)

project(qimen LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
include/qimen.h
src/qimen.cpp
src/qm_v1.h
src/qm_v1.cpp
src/qm_v2.h
src/qm_v2.cpp
src/qm_v3.h
src/qm_v3.cpp
src/qm_v4.h
src/qm_v4.cpp
)
include_directories(include)
add_library(qimen STATIC ${SOURCES})
target_link_libraries(qimen PRIVATE tyme4cpp)
target_include_directories(qimen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
