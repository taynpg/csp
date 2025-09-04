# 获取一些常见的本机系统（注意是本机，非交叉编译器的信息）信息
if (UNIX)
execute_process(
    COMMAND uname -a
    OUTPUT_VARIABLE UNAME_OUTPUT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(UNAME_OUTPUT MATCHES "x86_64")
    set(SYSTEM_ARCH "x86_64")
elseif(UNAME_OUTPUT MATCHES "aarch64")
    set(SYSTEM_ARCH "aarch64")
elseif(UNAME_OUTPUT MATCHES "armv7l")
    set(SYSTEM_ARCH "armv7l")
elseif(UNAME_OUTPUT MATCHES "i686")
    set(SYSTEM_ARCH "i686")
elseif(UNAME_OUTPUT MATCHES "riscv64")
    set(SYSTEM_ARCH "riscv64")
else()
    set(SYSTEM_ARCH "unknown")
endif()
if(UNAME_OUTPUT MATCHES "alpine" OR UNAME_OUTPUT MATCHES "Alpine") 
	set(SYSTEM_ARCH "musl_${SYSTEM_ARCH}")
endif()
elseif (WIN32)
    # 先调用 ver 命令
    execute_process(COMMAND cmd /c ver
                    OUTPUT_VARIABLE VER_OUTPUT
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(VER_OUTPUT MATCHES "XP")
        message(STATUS "Windows XP platform.")
        set(SYSTEM_ARCH "x86")
    else()
        execute_process(COMMAND cmd /c wmic os get osarchitecture
                        OUTPUT_VARIABLE WMIC_OUTPUT
                        OUTPUT_STRIP_TRAILING_WHITESPACE)
        if(WMIC_OUTPUT MATCHES "64")
            set(SYSTEM_ARCH "x86_64")
        else()
            set(SYSTEM_ARCH "x86")
        endif()
    endif()
else()
message(FATAL_ERROR "unknow system type.")
endif()
message(STATUS "SYSTEM_ARCH: ${SYSTEM_ARCH}")