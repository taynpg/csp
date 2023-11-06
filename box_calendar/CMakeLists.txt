cmake_minimum_required(VERSION 3.8)

project(box_calendar LANGUAGES CXX)
add_library(box_calendar SHARED 
    "calendar_v1.h" "calendar_v1.cpp" 
    "calendar_v1_data.h" "calendar_v1_data.cpp" 
    "calendar_base.h" "calendar_base.cpp"
    )
target_compile_definitions(box_calendar PRIVATE CPP_CALENDAR_LIB)
target_include_directories(box_calendar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})