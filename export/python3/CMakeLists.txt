cmake_minimum_required(VERSION 3.16)

project(csp_qm LANGUAGES CXX)

# pybind11 为 git submodule
add_subdirectory(pybind11)

pybind11_add_module(csp_qm csp_module.cpp)
target_include_directories(csp_qm PRIVATE ${CMAKE_SOURCE_DIR}/3rd)
target_link_libraries(csp_qm PRIVATE csp_core)
