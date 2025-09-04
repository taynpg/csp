cmake_minimum_required(VERSION 3.16)

project(csp-project VERSION 1.5.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    add_compile_options(/source-charset:utf-8)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "Csp Add MinGW Param.")
    add_compile_options(-finput-charset=utf-8)
    add_compile_options(-fexec-charset=gbk)
endif()

add_subdirectory(base)
add_subdirectory(zhcn)
add_subdirectory(tyme4cpp)
add_subdirectory(qimen)
add_subdirectory(src)

