#ifndef CAL_TABLE_H
#define CAL_TABLE_H

#include "calendar.h"
#include <vector>

namespace csp {
constexpr int cnum = 10;
class CalendarTable : public Calendar
{
public:
    CalendarTable();
    ~CalendarTable() override;

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
    CSPT days_difference(const DateTime& l, const DateTime& r) override;

    // 根据秒差值计算新日期。
    DateTime cal_new_date(const DateTime& base, CSPLT secs) override;

    // 获取距离 0点的秒数差。
    CSPLT cal_zero_secs(const DateTime& dt) override;

    // 获取两个日期时间之间的秒数差（不记入天），计算为 l - r。
    CSPT calsecs_by_time(const DateTime& l, const DateTime& r) override;

    // 返回两个时间之间的秒数差（记入天），计算为 l - r。
    CSPLT calsecs_by_all(const DateTime& l, const DateTime& r) override;

private:
    void init_data();

private:
    // 农历指定年所闰的月数，没有返回0
    CSPT get_leap_mon(CSPT year);

    // 返回农历 xx 年闰月的天数
    CSPT get_leap_days(CSPT year);

    // 返回农历 xx 年的总的天数
    CSPT get_lunar_days(CSPT year);

    // 返回农历 xx 年 xx 月的天数
    CSPT get_lunar_day(CSPT year, CSPT mon);

private:
    // 获取某年第 x 个节气为几号(从 0 小寒算起)
    CSPT get_lunar_sterm(CSPT year, CSPT cnt, DateTime& datetime);

    // 计算农历的年干支，月干支 (立春为新年)
    void cal_ym_ganzhi();

    // 计算时辰的干支(在计算完年月干支后进行)
    void cal_hour_ganzhi();

private:
    // 返回距离 1900.1.1 的天数
    static CSPT get_days_from_base(const DateTime& dt);

    // 返回下一个月的年干支月干支信息
    void next_mon_info(CSPT& year, CSPT& mon, CSPT& jie, CSPT& jz_mon, CSPT& jz_year);

    // 获取上一个月的年月干支，最后一个参数为当月是否是 立春
    void pre_mon_info(CSPT jz_year, CSPT jz_mon, CSPT& jz_year_out, CSPT& jz_mon_out, bool lichun);

    // 给定年干支，返回下一年的年干支和起始月干支
    void next_year_info(CSPT jz_year, CSPT& jz_year_out, CSPT& jz_mon_out);

private:
    std::vector<int> difference_jq_{};   // 计算节气的差值数组
    std::vector<int> lunar_table_{};     // 农历表数组
};

}   // namespace csp

#endif