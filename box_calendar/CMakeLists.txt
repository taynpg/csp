cmake_minimum_required(VERSION 3.8)

project(box_calendar LANGUAGES CXX)

include_directories(${CMAKE_SOURCE_DIR}/3rd/sxtwl_cpp/src)
add_library(box_calendar STATIC 
    "calendar_v1.h" 
    "calendar_v1.cpp" 
    "calendar_v1_data.h" 
    "calendar_v1_data.cpp" 
    "calendar_base.h" 
    "calendar_base.cpp"
    "calendar_v2.h" 
    "calendar_v2.cpp"
)
target_link_libraries(box_calendar PRIVATE sxtwl)
target_compile_definitions(box_calendar PRIVATE CPP_CALENDAR_LIB)
target_include_directories(box_calendar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})