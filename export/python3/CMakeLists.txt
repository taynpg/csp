cmake_minimum_required(VERSION 3.16)

project(csp_qm)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(pybind11)

# find_package (Python EXACT COMPONENTS Interpreter Development REQUIRED)
pybind11_add_module(csp_qm csp_module.cpp 
../../src/qmuse.cpp
../../src/print.cpp
)
target_link_libraries(csp_qm PRIVATE qimen zhcn base tyme4cpp)