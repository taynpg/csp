#include "csp/calendar.hpp"

#include <chrono>
#include <cstdio>
#include <ctime>

namespace csp::cal {

int wrap(int base, int value)
{
    return (value % base + base) % base;
}

DateTime now()
{
    using namespace std::chrono;

    const auto current = system_clock::now();
    const auto seconds = system_clock::to_time_t(current);
    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &seconds);
#else
    localtime_r(&seconds, &local);
#endif
    const auto millis = duration_cast<milliseconds>(current.time_since_epoch()) % 1000;

    DateTime dt;
    dt.year = local.tm_year + 1900;
    dt.mon = local.tm_mon + 1;
    dt.day = local.tm_mday;
    dt.hour = local.tm_hour;
    dt.min = local.tm_min;
    dt.sec = local.tm_sec;
    dt.msec = static_cast<int>(millis.count());
    return dt;
}

DateTime from_solar(const tyme::SolarTime& time)
{
    return DateTime(time.get_year(), time.get_month(), time.get_day(), time.get_hour(), time.get_minute(),
                    time.get_second());
}

DateTime from_lunar(const tyme::SolarTime& time)
{
    const auto lunar = time.get_lunar_hour();
    return DateTime(lunar.get_year(), lunar.get_month(), lunar.get_day(), lunar.get_hour(), lunar.get_minute(),
                    lunar.get_second());
}

tyme::SolarTime to_solar(const DateTime& dt)
{
    return tyme::SolarTime(dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec);
}

DateTime add_seconds(const DateTime& dt, int seconds)
{
    return from_solar(to_solar(dt).next(seconds));
}

int days_between(const DateTime& dt1, const DateTime& dt2)
{
    // 只按日期比较，因此把 dt1 的时分秒换成 dt2 的
    DateTime first = dt1;
    first.hour = dt2.hour;
    first.min = dt2.min;
    first.sec = dt2.sec;

    const auto seconds = to_solar(dt2).subtract(to_solar(first));
    return seconds / (24 * 60 * 60);
}

Pillars four_pillars(const tyme::SolarTime& time)
{
    const auto hour = time.get_sixty_cycle_hour();
    const auto day = hour.get_sixty_cycle_day();
    const auto month = day.get_sixty_cycle_month();
    const auto year = month.get_sixty_cycle_year();

    Pillars pillars;
    pillars.year = year.get_sixty_cycle().get_index();
    pillars.month = month.get_sixty_cycle().get_index();
    pillars.day = day.get_sixty_cycle().get_index();
    pillars.hour = hour.get_sixty_cycle().get_index();
    return pillars;
}

Pillars four_pillars_without_hour(const tyme::SolarTime& time)
{
    const auto day = tyme::SolarDay(time.get_year(), time.get_month(), time.get_day()).get_sixty_cycle_day();
    const auto month = day.get_sixty_cycle_month();
    const auto year = month.get_sixty_cycle_year();

    Pillars pillars;
    pillars.year = year.get_sixty_cycle().get_index();
    pillars.month = month.get_sixty_cycle().get_index();
    pillars.day = day.get_sixty_cycle().get_index();
    pillars.hour = 0;
    return pillars;
}

namespace {

/// 第 index 个节气（0 = 冬至）的交节时刻。
/// tyme 的节气序号与公历月份错开一个月，故按月份推算：
/// 1 月上半月的上一个节气是 1 号小寒，下半月的下一个节气是 2 号大寒，以此类推。
tyme::SolarTime term_time(int year, int index)
{
    const auto term = tyme::SolarTerm::from_index(year, index);
    return term.get_julian_day().get_solar_time();
}

Term term_around(const tyme::SolarTime& time, bool next)
{
    const int mon = time.get_month();
    int year = time.get_year();
    int index = next ? mon * 2 : mon * 2 - 1;
    if (index == kSolarTermCount) {
        // 12 月的下一个节气是次年 0 号（冬至）
        year += 1;
        index = 0;
    }
    return Term(index, term_time(year, index));
}

}   // namespace

TermSpan terms_around(const tyme::SolarTime& time)
{
    return TermSpan(term_around(time, false), term_around(time, true));
}

}   // namespace csp::cal
