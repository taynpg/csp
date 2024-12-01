cmake_minimum_required(VERSION 3.16)

project(csp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/source-charset:utf-8)
    # add_compile_options(/utf-8)
    # 以下抑制，主要作用与三方库。
    add_compile_options(/wd4018) #有符号与无符号的比较
    add_compile_options(/wd4800) #将值强制为布尔值“true”或“false”(性能警告)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "Add MinGW Param.")
    add_compile_options(-finput-charset=utf-8)
    add_compile_options(-fexec-charset=gbk)
endif()

message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler CXX ID: ${CMAKE_CXX_COMPILER_ID}")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

set(PROJECT_SOURCES
    src/csp_base.hpp
    src/qm_use.cpp
    src/qm_use.h
    src/print_base.cpp
    src/print_base.h
    src/main.cpp
)

include_directories(qm)
add_subdirectory(base)
add_subdirectory(calendar)
add_subdirectory(qimen)
add_subdirectory(sxtwl)
add_subdirectory(zhcn)

add_executable(csp ${PROJECT_SOURCES})
target_link_libraries(csp PRIVATE qimen zhcn calendar)
