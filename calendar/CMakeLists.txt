cmake_minimum_required(VERSION 3.16)

project(calendar LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED USE_UTF8_ALL)
if (MSVC)
    add_compile_options(/source-charset:utf-8)
endif()
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "calendar Add MinGW Param.")
    add_compile_options(-finput-charset=utf-8)
    add_compile_options(-fexec-charset=gbk)
endif()
endif()

set(CMAKE_DEBUG_POSTFIX "d")
include_directories(include)
include_directories(src)
set(SOURCES 
    src/calendar.cxx src/cal_table.cxx 
    src/cal_sx.cxx src/table_data.cxx
)
add_library(calendar STATIC ${SOURCES})
target_link_libraries(calendar PRIVATE sxtwl)
target_include_directories(calendar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (MSVC)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    string(REPLACE "/O2" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    target_compile_options(calendar PRIVATE /O1)
endif()
endif()