cmake_minimum_required(VERSION 3.16)

project(box_qimen)
if (MSVC)
    add_compile_options(/utf-8)
endif()
include_directories(../box_calendar)
add_library(box_qimen STATIC 
        "qm_base.h"
        "qm_base.cpp" 
        "qm_v1.h" 
        "qm_v1.cpp"
        "qm_v2.h" 
        "qm_v2.cpp"
        "qm_v3.h"
        "qm_v3.cpp"
        "qm_v4.h" 
        "qm_v4.cpp"
)
target_link_libraries(box_qimen box_calendar)
target_include_directories(box_qimen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
