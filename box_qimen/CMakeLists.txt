cmake_minimum_required(VERSION 3.8)

project(box_qimen)
include_directories(../box_calendar)
add_library(box_qimen SHARED "qm_base.h"
        "qm_base.cpp" "qm_v1.h" "qm_v1.cpp"
        "qm_v2.h" "qm_v2.cpp"
        qm_v3.h
        qm_v3.cpp)
target_compile_definitions(box_qimen PRIVATE CPP_QIMEN_LIB)
target_link_libraries(box_qimen box_calendar)
target_include_directories(box_qimen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
