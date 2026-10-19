cmake_minimum_required(VERSION 3.16)

project(csp VERSION 1.6.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "构建类型" FORCE)
endif()

option(CSP_BUILD_TESTS "构建单元测试" ON)
option(CSP_PYTHON_EXPORT "构建 Python 扩展模块" OFF)

# ------------------------------------------------------------------ 编译选项

if (MSVC)
    add_compile_options(/utf-8 /W4 /permissive-)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -DNOMINMAX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_SYSTEM_NAME MATCHES "Windows")
    message(STATUS "csp: 使用 MinGW 构建，安装时会一并拷贝运行时 DLL")
    get_filename_component(MINGW_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
    install(FILES
        "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
        "${MINGW_BIN_DIR}/libstdc++-6.dll"
        "${MINGW_BIN_DIR}/libwinpthread-1.dll"
        DESTINATION bin OPTIONAL)
endif()

# ------------------------------------------------------------------ 版本头

configure_file(include/csp/version.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/include/csp/version.hpp @ONLY)

# ------------------------------------------------------------------ 依赖

add_subdirectory(tyme4cpp)

# ------------------------------------------------------------------ 核心库

add_library(csp_core STATIC
    src/qimen/calendar.cpp
    src/qimen/qimen.cpp
    src/qimen/qimen_v1.cpp
    src/qimen/qimen_v2.cpp
    src/qimen/qimen_v3.cpp
    src/qimen/qimen_v4.cpp
    src/zh/names.cpp
    src/app/console.cpp
    src/app/options.cpp
    src/app/render_json.cpp
    src/app/render_text.cpp
    src/app/runner.cpp
)

target_include_directories(csp_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/3rd
)
target_link_libraries(csp_core PUBLIC tyme4cpp)
# fmt 与 nlohmann/json 均为 header-only，随源码分发
target_compile_definitions(csp_core PUBLIC FMT_HEADER_ONLY)
set_target_properties(csp_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

# ------------------------------------------------------------------ 可执行文件

add_executable(csp src/main.cpp)
target_include_directories(csp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/3rd)
target_link_libraries(csp PRIVATE csp_core)

if(UNIX)
    execute_process(COMMAND uname -a OUTPUT_VARIABLE UNAME_OUT OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(UNAME_OUT MATCHES "alpine" OR UNAME_OUT MATCHES "Alpine")
        message(STATUS "csp: 在 musl 环境下静态链接")
        target_link_libraries(csp PRIVATE -static -static-libstdc++)
    endif()
endif()

# ------------------------------------------------------------------ 测试

if (CSP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ------------------------------------------------------------------ Python 扩展

if (CSP_PYTHON_EXPORT)
    message(STATUS "csp: 构建 Python 扩展模块")
    if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/export/python3/pybind11/CMakeLists.txt)
        message(FATAL_ERROR "缺少 pybind11，请执行：git submodule update --init --recursive")
    endif()
    add_definitions(-DFMT_UNICODE=1)
    add_subdirectory(export/python3)
endif()

# ------------------------------------------------------------------ 安装

include(GNUInstallDirs)
install(TARGETS csp RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(TARGETS csp_core ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/csp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        PATTERN "version.hpp.in" EXCLUDE)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/csp/version.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/csp)

# ------------------------------------------------------------------ 打包

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH_BIT "64Bit")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ARCH_BIT "32Bit")
else()
    message(FATAL_ERROR "无法识别的目标架构")
endif()

# 目标架构（用于安装包命名）
string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" SYSTEM_ARCH)
if (SYSTEM_ARCH MATCHES "^(amd64|x86_64|x64)$")
    set(SYSTEM_ARCH "x86_64")
elseif (SYSTEM_ARCH MATCHES "^(aarch64|arm64)$")
    set(SYSTEM_ARCH "aarch64")
elseif (SYSTEM_ARCH MATCHES "^(i.86|x86)$")
    set(SYSTEM_ARCH "x86")
elseif (SYSTEM_ARCH STREQUAL "")
    set(SYSTEM_ARCH "unknown")
endif()

set(CPACK_PACKAGE_NAME "csp")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_CONTACT "taynpg <taynpg@163.com>")
set(CPACK_PACKAGE_DESCRIPTION "A Qimen Shipan.")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "奇门遁甲排盘工具（命令行）")
set(CPACK_PACKAGE_VENDOR "taynpg")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/taynpg/csp")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

if(WIN32)
    set(CPACK_NSIS_MODIFY_PATH ON)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(CPACK_GENERATOR "NSIS64;ZIP")
    else()
        set(CPACK_GENERATOR "NSIS;ZIP")
    endif()
    set(CPACK_NSIS_INSTALL_ROOT "$PROFILE")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "csp")
    set(CPACK_NSIS_DISPLAY_NAME "csp ${PROJECT_VERSION}")
    set(CPACK_NSIS_PACKAGE_NAME "csp")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
elseif(UNIX AND NOT APPLE)
    # .deb 用 Debian 惯例命名（csp_1.6.0_amd64.deb），可直接 dpkg -i
    set(CPACK_GENERATOR "DEB;TGZ")
    set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "taynpg <taynpg@163.com>")
    # 自动分析可执行文件的动态库依赖，避免装完缺库
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
    if(SYSTEM_ARCH STREQUAL "x86_64")
        set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    elseif(SYSTEM_ARCH STREQUAL "aarch64")
        set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "arm64")
    else()
        set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${SYSTEM_ARCH}")
    endif()
elseif(APPLE)
    # 命令行工具没有 .app，不需要 dmg，直接给 tar.gz
    set(CPACK_GENERATOR "TGZ")
endif()

if (MSVC)
    set(COMPILER_ID "msvc")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if (WIN32)
        set(COMPILER_ID "mingw")
    else()
        set(COMPILER_ID "gnu")
    endif()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(COMPILER_ID "clang")
else()
    set(COMPILER_ID ${CMAKE_CXX_COMPILER_ID})
endif()
string(TOLOWER "${COMPILER_ID}" COMPILER_ID_LOWER)

# 多架构（macOS universal）时用 universal 代替具体架构
if (APPLE AND CMAKE_OSX_ARCHITECTURES MATCHES ";")
    set(SYSTEM_ARCH "universal")
endif()

if (SYSTEM_ARCH STREQUAL "universal")
    set(CPACK_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}-v${PROJECT_VERSION}-${COMPILER_ID_LOWER}.${SYSTEM_ARCH}")
else()
    set(CPACK_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}-v${PROJECT_VERSION}-${COMPILER_ID_LOWER}.${SYSTEM_ARCH}.${ARCH_BIT}")
endif()

include(CPack)
