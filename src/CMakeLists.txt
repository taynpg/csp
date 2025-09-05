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
if(UNIX)
execute_process(
    COMMAND uname -a
    OUTPUT_VARIABLE UNAME_OUT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if(UNAME_OUT MATCHES "alpine" OR UNAME_OUT MATCHES "Alpine")
message(STATUS "csp on musl static link")
target_link_libraries(csp PRIVATE -static;-static-libstdc++)
endif()
endif()