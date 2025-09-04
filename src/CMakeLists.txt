cmake_minimum_required(VERSION 3.16)

project(csp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES 
CLI11.hpp
ColorConsole.hpp
csp_base.hpp
main.cpp
print.cpp
print.h
qmuse.cpp
qmuse.h
)
add_executable(csp ${SOURCES})
target_link_libraries(csp PRIVATE qimen zhcn base tyme4cpp)
