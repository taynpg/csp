#include <iostream>
#include "common/cmd_show.h"

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

int main()
{
#ifdef _WIN32
    // 获取标准输出句柄
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // 启用 ANSI 转义代码支持
    DWORD mode;
    GetConsoleMode(hConsole, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, mode);
#endif

    std::array<int, 6> array{1, 0, 1, 0, 1, 0};
    CGuaPrint print;
    print.show_six(array);

    return 0;
}