cmake_minimum_required(VERSION 3.16)

project(qm LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED USE_UTF8_ALL)
if (MSVC)
    add_compile_options(/source-charset:utf-8)
endif()
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "qimen Add MinGW Param.")
    add_compile_options(-finput-charset=utf-8)
    add_compile_options(-fexec-charset=gbk)
endif()
endif()

set(CMAKE_DEBUG_POSTFIX "d")
include_directories(include)
include_directories(src)
set(SOURCES 
    src/qimen.cxx src/qm_v1.cxx
    src/qm_v2.cxx src/qm_v3.cxx src/qm_v4.cxx
)
add_library(qm STATIC ${SOURCES})
target_link_libraries(qm PRIVATE calendar)
target_include_directories(qm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)