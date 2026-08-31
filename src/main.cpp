//
// Created by Administrator on 2023-10-07.
//

#include <cstdio>
#include <fmt/color.h>
#include <fmt/format.h>
#include <regex>


#include "csp_base.hpp"
#include "qmuse.h"
#include <CLI11.hpp>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

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

#ifdef _WIN32
static HANDLE hOut = nullptr;
class u8ConsoleBuf : public std::streambuf
{
protected:
    int overflow(int c = EOF) override
    {
        if (c != EOF) {
            char ch = static_cast<char>(c);
            xsputn(&ch, 1);
        }
        return c;
    }
    std::streamsize xsputn(const char* s, std::streamsize n) override
    {
        int wideLen = MultiByteToWideChar(CP_UTF8, 0, s, static_cast<int>(n), nullptr, 0);
        if (wideLen <= 0) {
            return 0;
        }

        std::vector<wchar_t> buffer(wideLen);
        MultiByteToWideChar(CP_UTF8, 0, s, static_cast<int>(n), buffer.data(), wideLen);

        if (hOut == nullptr) {
            hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        }

        DWORD written;
        WriteConsoleW(hOut, buffer.data(), wideLen, &written, nullptr);

        return n;
    }
};
#endif

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
        fmt::print("日期字符串格式不正确。\n");
        return false;
    }
    return true;
}

bool cmd(int argc, char** argv, CmdParam& param)
{
#ifdef _WIN32
    static u8ConsoleBuf u8Buf;
    std::cout.rdbuf(&u8Buf);
#endif
    set_output_supply();
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

    app.add_option("--ju", param.ju, "局数(默认自动局数)");
    app.add_option("-z,--zone", param.zone, "与现在的时间差值(单位为小时，默认为0)");
    app.add_option("-a,--angan", param.angan,
                   "暗干排法（默认0）：\n"
                   "[0,门原宫位地盘]\n"
                   "[1,入值使门]");

    app.add_flag("-j,--json", param.exportJson, "导出json文件");

    try {
        CLI11_PARSE(app, argc, argv);
        return true;
    } catch (const CLI::ParseError& e) {
        fmt::print(stderr, "Error parsing command line: {}\n", e.what());
        return false;
    }
    return false;
}

int main(int argc, char** argv)
{
    CmdParam param;

    if (!cmd(argc, argv, param)) {
        return 0;
    }

    if (param.type == -1) {
        fmt::print("盘式类型为必填项，-t，可使用--help查看帮助。\n");
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
        fmt::print("未匹配到支持的盘面类型，请使用 --help 查看帮助。\n");
    }
    }
    return 0;
}
