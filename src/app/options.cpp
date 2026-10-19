#include "csp/app.hpp"

#include <regex>

#include "csp/calendar.hpp"

namespace csp {

bool parse_datetime_text(const std::string& text, DateTime& out, std::string* error)
{
    static const std::regex pattern(R"((-?\d{1,4})-(\d{1,2})-(\d{1,2})-(\d{1,2})-(\d{1,2})-(\d{1,2}))");
    std::smatch match;
    if (!std::regex_search(text, match, pattern)) {
        if (error != nullptr) {
            *error = "日期格式不正确，应为 YYYY-MM-DD-HH-MM-SS。";
        }
        return false;
    }

    out.year = std::stoi(match[1].str());
    out.mon = std::stoi(match[2].str());
    out.day = std::stoi(match[3].str());
    out.hour = std::stoi(match[4].str());
    out.min = std::stoi(match[5].str());
    out.sec = std::stoi(match[6].str());
    return true;
}

bool validate(Options& options, std::string* error)
{
    const auto fail = [error](const std::string& message) {
        if (error != nullptr) {
            *error = message;
        }
        return false;
    };

    if (options.auto_time) {
        options.time = cal::now();
    }

    const DateTime& t = options.time;
    if (t.year < 2 || t.year > 9998) {
        return fail("年份超出支持范围（2-9998）。");
    }
    if (t.mon < 1 || t.mon > 12) {
        return fail("月份应在 1-12 之间。");
    }
    if (t.day < 1 || t.day > 31) {
        return fail("日期应在 1-31 之间。");
    }
    if (t.hour < 0 || t.hour > 23) {
        return fail("小时应在 0-23 之间。");
    }
    if (t.min < 0 || t.min > 59) {
        return fail("分钟应在 0-59 之间。");
    }
    if (t.sec < 0 || t.sec > 59) {
        return fail("秒应在 0-59 之间。");
    }
    if (options.ju < -9 || options.ju > 9) {
        return fail("局数应在 -9 到 9 之间（0 表示自动定局）。");
    }
    if (options.zone_hours < -24 || options.zone_hours > 24) {
        return fail("时差应在 -24 到 24 小时之间。");
    }
    return true;
}

}   // namespace csp
