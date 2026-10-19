// csp - 奇门遁甲排盘库
// 日历工具：对 tyme4cpp 的一层薄封装，屏蔽其调用链较长的接口。
#ifndef CSP_CALENDAR_HPP
#define CSP_CALENDAR_HPP

#include <tyme.h>

#include "csp/types.hpp"

namespace csp::cal {

/// 取模，结果恒为非负（value 可为负）
int wrap(int base, int value);

/// 当前本地时间
DateTime now();

/// 公历时间 -> DateTime
DateTime from_solar(const tyme::SolarTime& time);

/// 农历时间 -> DateTime（月份为负表示闰月）
DateTime from_lunar(const tyme::SolarTime& time);

/// DateTime -> 公历时间（非法时间会抛出 std::exception）
tyme::SolarTime to_solar(const DateTime& dt);

/// 时间偏移
DateTime add_seconds(const DateTime& dt, int seconds);

/// 两个时间相差的天数（忽略 dt1 的时分秒，只按日期算）
int days_between(const DateTime& dt1, const DateTime& dt2);

/// 四柱，下标为六十甲子序号
Pillars four_pillars(const tyme::SolarTime& time);

/// 四柱（时柱以 0 填充，仅用于按日取年月日三柱的场景）
Pillars four_pillars_without_hour(const tyme::SolarTime& time);

/// 一个节气：序号（0 = 冬至）与交节时刻
struct Term {
    Term(int index, const tyme::SolarTime& time) : index(index), time(time)
    {
    }

    int index{};
    tyme::SolarTime time;
};

/// 某一时刻前后相邻的两个节气
struct TermSpan {
    TermSpan(const Term& previous, const Term& next) : previous(previous), next(next)
    {
    }

    Term previous;
    Term next;
};

/// 取某时刻前后相邻的节气。
/// 上半月取上个月的节气，即按公历月份推算，故年初/年末会跨年。
TermSpan terms_around(const tyme::SolarTime& time);

}   // namespace csp::cal

#endif   // CSP_CALENDAR_HPP
