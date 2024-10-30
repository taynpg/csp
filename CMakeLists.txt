cmake_minimum_required(VERSION 3.16)

project(csp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/source-charset:utf-8)
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
    csp_base.hpp
    qm/qm_use.cpp
    qm/qm_use.h
    qm/print_base.cpp
    qm/print_base.h
    main.cpp
)

set(TEST_SOURCES
    test.cpp
    common/cmd_show.h
    common/cmd_show.cpp
)

include_directories(qm)
include_directories(${CMAKE_SOURCE_DIR}/3rd/CLI11)
add_subdirectory(box_calendar)
add_subdirectory(box_qimen)
add_subdirectory(box_yxbase)
add_subdirectory(box_zhdata)
add_subdirectory(3rd/sxtwl_cpp)

add_executable(csp ${PROJECT_SOURCES})
add_executable(csp_test ${TEST_SOURCES})
target_link_libraries(csp box_qimen box_zhdata box_calendar)
