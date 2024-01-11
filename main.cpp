//
// Created by Administrator on 2023-10-07.
//

#include <CLI11.hpp>
#include <iostream>
#include <regex>

#include "csp_base.hpp"
#include "qm_use.h"

bool ParseDate(const std::string& str, CMDParam& param)
{
    std::regex dateRegex(
        R"((-?\d{1,4})-(\d{1,2})-(\d{1,2})-(\d{1,2})-(\d{1,2})-(\d{1,2}))");
    std::smatch match;
    bool        is_valid = false;
    // 使用正则表达式进行匹配
    if (std::regex_search(str, match, dateRegex)) {
        param.datetime.m_date.m_nYear = std::stoi(match[1].str());
        param.datetime.m_date.m_nMon = std::stoi(match[2].str());
        param.datetime.m_date.m_nDay = std::stoi(match[3].str());
        param.datetime.m_time.m_nHour = std::stoi(match[4].str());
        param.datetime.m_time.m_nMin = std::stoi(match[5].str());
        param.datetime.m_time.m_nSec = std::stoi(match[6].str());
        if (cppbox::CCalenderBase::checkFormatOnly(param.datetime)) {
            is_valid = true;
        }
    }

    if (!is_valid) {
        std::cout << "日期格式不正确。" << std::endl;
        return false;
    }

    return true;
}

bool cmd(int argc, char** argv, CMDParam& param)
{
    std::string intro("命令行排盘工具 csp  ");
    intro.append(CSP_VERSION);
    CLI::App app(intro);

    app.add_option("-t,--type", param.nType,
                   "==> 盘式类型(必填) <==\n"
                   "[1,时家转盘超接置润]\n"
                   "[2,时家转盘阴盘]\n"
                   "[3,时家转盘拆补]");

    app.add_option("-d,--date", param.str_datetime,
                   "==> 输入日期(默认当前时间) <==\n"
                   "手动输入格式: 2000-2-3-15-32-11");

    app.add_option("-c,--calendar", param.calendar_type,
                   "==> 日历类型(默认寿星天文历) <==\n"
                   "[0,查表法(1901-1-31~2099-12-31)]\n"
                   "[1,天文历(-198-1-1~9999-12-31)]");

    app.add_option("-j,--ju", param.nJu, "局数(默认自动局数)");

    try {
        CLI11_PARSE(app, argc, argv);
        return true;
    } catch (const CLI::ParseError& e) {
        std::cerr << "Error parsing command line: " << e.what() << std::endl;
        return false;
    }
}

int main(int argc, char** argv)
{
    CMDParam param;
    if (!cmd(argc, argv, param)) {
        return 0;
    }

    if (param.nType == -1) {
        std::cout << "盘式类型为必填项，-t，可使用--help查看帮助。"
                  << std::endl;
        return -1;
    }

    if ((param.calendar_type != 0) && (param.calendar_type != 1)) {
        std::cout << "日历类型不正确，可选项[0,1]，可使用--help查看帮助。"
                  << std::endl;
        return -1;
    }

    if (!param.str_datetime.empty() && !ParseDate(param.str_datetime, param)) {
        return -1;
    }

    switch (param.nType) {
        case 1:
        case 2:
        case 3: {
            CQimenUse qmuse;
            if (param.str_datetime.empty()) {
                param.isAutoDate = true;
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
