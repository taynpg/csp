#include "calendar.h"
#include "cal_sx.h"
#include "cal_table.h"
#include <ctime>

namespace csp {

Calendar::Calendar()
{
    wuhu_[0] = 2;
    wuhu_[1] = 4;
    wuhu_[2] = 6;
    wuhu_[3] = 8;
    wuhu_[4] = 0;
    wuhu_[5] = 2;
    wuhu_[6] = 4;
    wuhu_[7] = 6;
    wuhu_[8] = 8;
    wuhu_[9] = 0;

    qishi_[0] = 0;
    qishi_[1] = 2;
    qishi_[2] = 4;
    qishi_[3] = 6;
    qishi_[4] = 8;
    qishi_[5] = 0;
    qishi_[6] = 2;
    qishi_[7] = 4;
    qishi_[8] = 6;
    qishi_[9] = 8;
}

std::shared_ptr<Calendar> Calendar::instance(CalendarType type)
{
    std::shared_ptr<Calendar> r = nullptr;
    switch (type) {
    case CalendarType::CAL_TYPE_TABLE:
        r = std::make_shared<CalendarTable>();
        break;
    case CalendarType::CAL_TYPE_SXTWL:
        r = std::make_shared<CalendarSxwnl>();
        break;
    default:
        break;
    }
    return r;
}

bool Calendar::check_format_only(const DateTime& dt)
{
    if (dt.mon < 1 || dt.mon > 12) {
        return false;
    }

    int y = dt.year;
    int m = dt.mon;
    int d = dt.day;

    if (d < 1) {
        return false;
    }

    if (is_leap(y)) {
        if (m == 2) {
            if (d > 29) {
                return false;
            }
        }
    } else {
        if (m == 2) {
            if (d > 28) {
                return false;
            }
        }
    }

    if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) {
        if (d > 31) {
            return false;
        }
    }

    if (m == 4 || m == 6 || m == 9 || m == 11) {
        if (d > 30) {
            return false;
        }
    }

    int h = dt.hour;
    int min = dt.min;
    int s = dt.sec;

    if (h > 23 || h < 0) {
        return false;
    }
    if (min > 59 || min < 0) {
        return false;
    }
    if (s > 59 || s < 0) {
        return false;
    }

    return true;
}

bool Calendar::is_leap(CSPT year)
{
    if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)) {
        return true;
    }
    return false;
}

DateTime Calendar::now()
{
    DateTime r{};
#ifdef _WIN32
    SYSTEMTIME nt;
    GetLocalTime(&nt);
    r.year = nt.wYear;
    r.mon = nt.wMonth;
    r.day = nt.wDay;
    r.hour = nt.wHour;
    r.min = nt.wMinute;
    r.sec = nt.wSecond;
#else
    struct std::tm* nt = nullptr;
    std::time_t _otime_t;
    std::time(&_otime_t);
    nt = std::localtime(&_otime_t);
    r.year = nt->tm_year + 1900;
    r.mon = nt->tm_mon + 1;
    r.day = nt->tm_mday;
    r.hour = nt->tm_hour;
    r.min = nt->tm_min;
    r.sec = nt->tm_sec;
#endif
    return r;
}

CSPT Calendar::mod(CSPT base, CSPT value)
{
    return (value % base + base) % base;
}

bool Calendar::is_same_day(const DateTime& d, const DateTime& p)
{
    if (d.year != p.year) {
        return false;
    }
    if (d.mon != p.mon) {
        return false;
    }
    if (d.day != p.day) {
        return false;
    }
    return true;
}

bool Calendar::is_same_all(const DateTime& d, const DateTime& p)
{
    if (!is_same_day(d, p)) {
        return false;
    }
    if (!is_same_time(d, p)) {
        return false;
    }
    return true;
}

bool Calendar::is_same_time(const DateTime& d, const DateTime& p)
{
    if (d.hour != p.hour) {
        return false;
    }
    if (d.min != p.min) {
        return false;
    }
    if (d.sec != p.sec) {
        return false;
    }
    return true;
}

DateTime Calendar::get_solar() const
{
    return solar_;
}

DateTime Calendar::get_lunar() const
{
    return lunar_;
}

std::array<JieQi, 6> Calendar::get_jq_info() const
{
    return jie_;
}

GanZhi Calendar::get_ganzhi() const
{
    return ganzhi;
}

DateTime::DateTime(CSPT y, CSPT m, CSPT d)
{
    year = y;
    mon = m;
    day = d;
}

DateTime::DateTime(CSPT y, CSPT m, CSPT d, CSPT h, CSPT mi, CSPT s)
{
    year = y;
    mon = m;
    day = d;
    hour = h;
    min = mi;
    sec = s;
}

}   // namespace csp
