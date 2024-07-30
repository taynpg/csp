#ifndef BOX_CALENDAR_HEADER_V2
#define BOX_CALENDAR_HEADER_V2

#include "calendar_base.h"

#include <const.h>
#include <sxtwl.h>

namespace cppbox {

class CCalenderV2 : public CCalenderBase
{
public:
    CCalenderV2();
    ~CCalenderV2() override;

public:
    bool set_datetime(const CDateTime& datetime) override;
    // 获取前一天
    void pre(CDateTime& datetime) override;
    void pre(CDate& date) override;
    // 获取后一天
    void next(CDateTime& datetime) override;
    void next(CDate& date) override;
    // 检查日期格式是否正确
    bool check_format(const CDateTime& datetime) override;
    // 返回两个日期之间的天数差
    int get_diff_day(const CDate& dateA, const CDate& dateB) override;
    // 返回距离 00:00:00 的秒数
    int get_sec_by_base(const CTime& time) override;
    // 基于基础时间和差值计算新的日期
    void get_diff_sec(const CDateTime& basetime, CDateTime& outtime, long long nSecond) override;
    // 返回两个时间之间的秒数差
    int get_diff_by_time(const CTime& timeA, const CTime& timeB) override;
    // 返回两个日期时间的秒数差
    long long get_sec_by_date(const CDateTime& datetimeA, const CDateTime& datetimeB) override;

private:
    void sx2dt(Time& time, CDateTime& datetime);
    void sx2lunar(Day* day, CDate& date);
    void sx2solar(Day* day, CDate& date);
    int rd(double number);
    // 根据节气时间修正月干支
    void check_mon_gz();

private:
    Day* day_{};
};

}   // namespace cppbox
#endif