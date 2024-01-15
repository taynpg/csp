#include "cmd_show.h"
#include <iostream>

// 定义颜色宏
#define BLACK_TEXT "\033[30m"
#define RED_TEXT "\033[31m"
#define GREEN_TEXT "\033[32m"
#define YELLOW_TEXT "\033[33m"
#define BLUE_TEXT "\033[34m"
#define MAGENTA_TEXT "\033[35m"
#define CYAN_TEXT "\033[36m"
#define WHITE_TEXT "\033[37m"

#define BLACK_BG "\033[40m"
#define RED_BG "\033[41m"
#define GREEN_BG "\033[42m"
#define YELLOW_BG "\033[43m"
#define BLUE_BG "\033[44m"
#define MAGENTA_BG "\033[45m"
#define CYAN_BG "\033[46m"
#define WHITE_BG "\033[47m"

#define RESET_COLOR "\033[0m"

CGuaPrint::~CGuaPrint() {}

void CGuaPrint::show(bool qian)
{
    if (qian) {
        std::cout << YELLOW_BG << "             " << RESET_COLOR << "\n";
        return;
    }
    std::cout << YELLOW_BG << "     " << RESET_COLOR << "   ";
    std::cout << YELLOW_BG << "     " << RESET_COLOR << "\n";
}

void CGuaPrint::show_six(const std::array<int, 6>& array) 
{
    std::cout << "\n";
    for (int i = 5; i >= 0; --i) {
        int value = array[i];
        if (value == 0) {
            show(false);
        } else {
            show(true);
        }
        std::cout << "\n";
    }
}
