//
// Created by Administrator on 2023-10-07.
//

#include <iostream>
#include <regex>

#include "CLI11.hpp"
#include "csp_base.hpp"
#include "qmuse.h"

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

void recovery_terminal_color()
{
#if defined(_WIN32)
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    if (h_console == NULL) {
        fprintf(stderr, "[ERROR] Couldn't handle terminal\n");
        exit(1);
    }
    if (SetConsoleTextAttribute(h_console, 7) == 0) {
        fprintf(stderr, "[ERROR] Couldn't set terminal color\n");
        exit(1);
    }
#else
    printf("\033[0m");
#endif
}

bool parse(const std::string& str, CmdParam& param)
{
    std::regex dateRegex(R"((-?\d{1,4})-(\d{1,2})-(\d{1,2})-(\d{1,2})-(\d{1,2})-(\d{1,2}))");
    std::smatch match;
    bool is_valid = false;
    // 使用正则表达式进行匹配
    if (std::regex_search(str, match, dateRegex)) {
        param.year = std::stoi(match[1].str());
        param.mon = std::stoi(match[2].str());
        param.day = std::stoi(match[3].str());
        param.hour = std::stoi(match[4].str());
        param.min = std::stoi(match[5].str());
        param.sec = std::stoi(match[6].str());
    } else {
        std::cout << "日期字符串格式不正确。" << std::endl;
        return false;
    }
    return true;
}

bool cmd(int argc, char** argv, CmdParam& param)
{
    std::string intro("命令行排盘工具 csp ");
    intro.append(CSP_VERSION);
    intro.append(" => https://github.com/taynpg/csp");
    CLI::App app(intro);

    app.add_option("-t,--type", param.type,
                   "==> 盘式类型(必填) <==\n"
                   "[1,时家转盘超接置润]\n"
                   "[2,时家转盘阴盘]\n"
                   "[3,时家转盘拆补]\n"
                   "[4,时家茅山]");

    app.add_option("-d,--date", param.str_dt,
                   "==> 输入日期(默认当前时间) <==\n"
                   "手动输入格式: 2000-2-3-15-32-11");

    app.add_option("-c,--calendar", param.cal_type,
                   "==> 日历类型(默认寿星天文历) <==\n"
                   "[0,查表法(1901-1-31~2099-12-31)]\n"
                   "[1,天文历(-198-1-1~9999-12-31)]");

    app.add_option("-j,--ju", param.ju, "局数(默认自动局数)");
    app.add_option("-z,--zone", param.zone, "与现在的时间差值(单位为小时，默认为0)");

    try {
        CLI11_PARSE(app, argc, argv);
        return true;
    } catch (const CLI::ParseError& e) {
        std::cerr << "Error parsing command line: " << e.what() << std::endl;
        return false;
    }
    return false;
}

void set_output_supply()
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
}

int main(int argc, char** argv)
{

#ifdef _WIN32
    system("chcp 65001");
#endif

    std::shared_ptr<int> deleter(new int(), [](int* p) {
        delete p;
        recovery_terminal_color();
    });

    CmdParam param;
    set_output_supply();

    if (!cmd(argc, argv, param)) {
        return 0;
    }

    if (param.type == -1) {
        std::cout << "盘式类型为必填项，-t，可使用--help查看帮助。" << std::endl;
        return -1;
    }

    if ((param.cal_type != 0) && (param.cal_type != 1)) {
        std::cout << "日历类型不正确，可选项[0,1]，可使用--help查看帮助。" << std::endl;
        return -1;
    }

    if (!param.str_dt.empty() && !parse(param.str_dt, param)) {
        return -1;
    }
    switch (param.type) {
    case 1:
    case 2:
    case 3:
    case 4: {
        CQimenUse qmuse;
        if (param.str_dt.empty()) {
            param.is_auto = true;
        }
        qmuse.run(param);
        break;
    }
    default: {
        std::wcout << L"未匹配到支持的盘面类型，请使用 --help 查看帮助。";
    }
    }
    return 0;
}
