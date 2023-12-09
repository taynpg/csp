//
// Created by Administrator on 2023-10-07.
//

#include <CLI11.hpp>
#include <iostream>
#include <regex>

#include "csp_base.hpp"
#include "qm_use.h"

bool ParseDate(const std::string& str, CMDParam& param) {
    std::regex dateRegex(R"(\d{4}-\d{1,2}-\d{1,2}-\d{1,2}-\d{1,2}-\d{1,2})");
    // 使用正则表达式进行匹配
    if (std::regex_match(str, dateRegex)) {
        // 正则表达式匹配成功，解析日期
        std::istringstream dateStream(str);
        int                year, month, day, hour, minute, second;
        char               dash;

        dateStream >> year >> dash >> month >> dash >> day >> dash >> hour >>
            dash >> minute >> dash >> second;

        // 检查日期的合法性
        struct tm tm {};
        tm.tm_year = year - 1900;  // 年份需要减去1900
        tm.tm_mon = month - 1;     // 月份需要减去1
        tm.tm_mday = day;
        tm.tm_hour = hour;
        tm.tm_min = minute;
        tm.tm_sec = second;

        if (mktime(&tm) != -1) {
            param.year = year;
            param.mon = month;
            param.day = day;
            param.hour = hour;
            param.min = minute;
            param.sec = second;
        } else {
            return false;
        }
    } else {
        std::cout << "日期格式不正确。" << std::endl;
        return false;
    }

    return true;
}

bool cmd(int argc, char** argv, CMDParam& param) {
    std::string intro("命令行排盘工具 csp  ");
    intro.append(CSP_VERSION);
    CLI::App app(intro);

    app.add_option("-t,--type", param.nType,
                   "盘式类型(必填)\n"
                   "[1,时家转盘超接置润]\n"
                   "[2,时家转盘阴盘]");

    app.add_option("-d,--date", param.datetime_,
                   "输入日期(默认当前时间)\n"
                   "格式: 2000-2-3-15-32-11");

    app.add_option("-j,--ju", param.nJu, "局数(默认自动局数)");
    CLI11_PARSE(app, argc, argv);
    return true;
}

int main(int argc, char** argv) {
    CMDParam param;
    if (!cmd(argc, argv, param)) {
        return 0;
    }

    if (param.nType == -1) {
        std::cout << "盘式类型为必填项，-t，可使用--help查看帮助。"
                  << std::endl;
        return -1;
    }

    if (!param.datetime_.empty() && !ParseDate(param.datetime_, param)) {
        return -1;
    }

    switch (param.nType) {
        case 1:
        case 2: {
            CQimenUse qmuse;
            if (param.year == 0) {
                FillTime(param);
            }
            qmuse.Run(param);
            break;
        }
        default: {
            std::cout << "未匹配到支持的盘面类型，请使用 --help 查看帮助。";
        }
    }
    return 0;
}
