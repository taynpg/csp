cmake_minimum_required(VERSION 3.8)

project(box_base)
add_library(box_base SHARED "box_base.h" "box_base.cpp")
target_link_libraries(box_base PRIVATE ${Boost_LIBRARIES})
target_compile_definitions(box_base PRIVATE CPP_BOXBASE_LIB)
target_include_directories(box_base PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
