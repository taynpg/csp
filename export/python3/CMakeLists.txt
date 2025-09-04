cmake_minimum_required(VERSION 3.16)

project(csp-qm)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(pybind11)

pybind11_add_module(csp-qm csp-module.cpp 
../../src/qmuse.cpp
../../src/print.cpp
)
target_link_libraries(csp-qm PRIVATE qimen zhcn base tyme4cpp)