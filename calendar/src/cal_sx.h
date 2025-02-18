#ifndef CAL_SXWNL
#define CAL_SXWNL

#include "calendar.h"
#include <const.h>
#include <sxtwl.h>

namespace csp {
class CalendarSxwnl : public Calendar
{
public:
    CalendarSxwnl();
    ~CalendarSxwnl() override;

public:
    // 设置某个时间点，并计算相关内容。一般最先调用这个，再使用其他函数。
    bool generate(const DateTime& dt) override;

    // 是否支持此日期
    bool is_support(const DateTime& dt) override;

    // 获取前 cnt 天的日期。
    void pre_day(DateTime& dt, unsigned int cnt) override;

    // 获取后 cnt 天的日期。
    void next_day(DateTime& dt, unsigned int cnt) override;

    // 获取两个时间之间的天数差，计算为 l - r。
    virtual CSPT days_difference(const DateTime& l, const DateTime& r) override;

    // 根据秒差值计算新日期。
    virtual DateTime cal_new_date(const DateTime& base, CSPLT secs) override;

    // 获取距离 0点的秒数差。
    virtual CSPLT cal_zero_secs(const DateTime& dt) override;

    // 获取两个日期时间之间的秒数差（不记入天），计算为 l - r。
    virtual CSPT calsecs_by_time(const DateTime& l, const DateTime& r);

    // 返回两个时间之间的秒数差（记入天），计算为 l - r。
    virtual CSPLT calsecs_by_all(const DateTime& l, const DateTime& r);

private:
    void sx2dt(const Time& t, DateTime& dt);
    void sx2lunar(Day* day, DateTime& date);
    void sx2solar(Day* day, DateTime& date);
    int rd(double number);
    // 根据节气时间修正月干支
    void check_mon_gz();

private:
    std::shared_ptr<Day> day_{};
};
}   // namespace csp

#endif