//
// Created by Administrator on 2023-10-07.
//

#include <iostream>
#include <regex>
#include <boost/program_options.hpp>
#include "csp_base.hpp"
#include "qm_use.h"

bool ParseDate(const std::string& str, CMDParam& param)
{
    std::regex dateRegex(R"(\d{4}-\d{1,2}-\d{1,2}-\d{1,2}-\d{1,2}-\d{1,2})");

    // 使用正则表达式进行匹配
    if (std::regex_match(str, dateRegex)) {

        // 正则表达式匹配成功，解析日期
        std::istringstream dateStream(str);
        int year, month, day, hour, minute, second;
        char dash;
        
        dateStream >> year >> dash >> month >> dash >> day >> dash >> hour >> dash >> minute >> dash >> second;
        
        // 检查日期的合法性
        struct tm tm{};
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

bool cmd(int argc, char** argv, CMDParam& param)
{
    namespace opt = boost::program_options;
    try {
        std::string baseInfo("命令行排盘工具 csp ");
        baseInfo += std::string(CSP_VERSION);
        opt::options_description des(baseInfo);
        des.add_options()
            ("help", "帮助信息\n"
            "示例:csp --t 0\n"
            "示例:csp --t 0 --date 1987-8-2-13-45-11")
            ("date", opt::value<std::string>(), "==> 输入日期(默认当前时间)\n"
            "格式:2000-2-3-15-32-11")
            ("t", opt::value<int>(), "==> 盘式类型【必填】\n"
            "涉及到的参数: --date, --j\n"
            "[0,时家转盘超接置润]\n"
            "[1,时家转盘阴盘]")
            ("j", opt::value<int>(), "==> 局数(默认自动局数)");

        opt::variables_map vm;
        opt::store(opt::parse_command_line(argc, argv, des), vm);
        opt::notify(vm);

        bool inputParam = false;

        if (vm.count("help")) {
            inputParam = true;
            std::cout << des << std::endl;
            return true;
        }
        if (vm.count("t")) {
            inputParam = true;
            param.nType = vm["t"].as<int>();
        }
        if (vm.count("j")) {
            inputParam = true;
            param.nJu = vm["j"].as<int>();
        }
        if (vm.count("date")) {
            inputParam = true;
            std::string dateStr = vm["date"].as<std::string>();
            if (!ParseDate(dateStr, param)) {
                return false;
            }
        }

        if (param.nType == -1 && inputParam) {
            std::cout << "--t 为必填项，请使用 --help 查看帮助。\n";
            std::cout << des << std::endl;
            return true;
        }

        if (!inputParam) {
            param.nType = -2;
            std::cout << des << std::endl;
            return true;
        }

    } catch (const std::exception& ex) {
        std::cerr << "错误: " << ex.what() << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char** argv)
{
    CMDParam param;
    if(!cmd(argc, argv, param)) {
        return 0;
    }

    switch (param.nType) {
    case 0:
    case 1:
    {
        CQimenUse qmuse;
        if (param.year == 0) {
            FillTime(param);
        }
        qmuse.Run(param);
        break;
    }
    case -2:
    {
        break;
    }
    default:
        std::cout << "未匹配到支持的盘面类型，请使用 --help 查看帮助。";
    }
    return 0;
}