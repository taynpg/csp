cmake_minimum_required(VERSION 3.5)

project(csp VERSION 1.1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(APEEND_THID_LIB_DIR
        "C:/bin/Boost"
        "/Users/tangyunpeng/mdev/Boost/"
        "/data/data/com.termux/files/home/dev/Boost"
)
list(APPEND CMAKE_PREFIX_PATH ${APEEND_THID_LIB_DIR})

if (MSVC)
    add_compile_options(/source-charset:utf-8)
endif()

#if (MSVC)
#    if (EXE_GBK)
#        add_compile_options(/source-charset:utf-8)
#    else ()
#        add_compile_options(/utf-8)
#    endif ()
#endif()

message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler C ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiler CXX ID: ${CMAKE_CXX_COMPILER_ID}")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/out)

set(PROJECT_SOURCES
        csp_base.hpp
        qm/qm_use.cpp
        qm/qm_use.h
        qm/print_base.cpp
        qm/print_base.h
        main.cpp
)

set(Boost_USE_STATIC_LIBS on)
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    find_package(Boost REQUIRED locale program_options)
    add_definitions(-D_WIN32_WINNT=0x0601)
else()
    find_package(Boost REQUIRED program_options)
endif()

message(STATUS "Boost Header: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost Libs: ${Boost_LIBRARIES}")
include_directories(${Boost_INCLUDE_DIRS})

include_directories(qm)
add_subdirectory(box_calendar)
add_subdirectory(box_base)
add_subdirectory(box_qimen)
add_subdirectory(box_yxbase)
add_subdirectory(box_zhdata)

add_executable(csp ${PROJECT_SOURCES})
target_link_libraries(csp box_qimen box_base box_zhdata box_calendar ${Boost_LIBRARIES})
