#ifndef CALENDAR_H
#define CALENDAR_H

#include <array>
#include <memory>
namespace csp {

using CSPT = int;
using CSPLT = long long;
enum class CalendarType {
    CAL_TYPE_TABLE,   // 查表法
    CAL_TYPE_SXTWL    // 寿星天文历
};

struct GanZhi {
    CSPT yg{};   // 年干
    CSPT yz{};   // 年支
    CSPT mg{};   // 月干
    CSPT mz{};   // 月支
    CSPT dg{};   // 日干
    CSPT dz{};   // 日支
    CSPT hg{};   // 时干
    CSPT hz{};   // 时支
};

struct DateTime {
    DateTime() = default;
    DateTime(CSPT y, CSPT m, CSPT d);
    DateTime(CSPT y, CSPT m, CSPT d, CSPT h, CSPT mi, CSPT s);
    CSPT year{};   // 年
    CSPT mon{};    // 月
    CSPT day{};    // 日
    CSPT hour{};   // 时
    CSPT min{};    // 分
    CSPT sec{};    // 秒
    CSPT msec{};   // 毫秒
};

struct JieQi {
    DateTime dt{};
    CSPT index{};
};

class Calendar
{
public:
    Calendar();
    virtual ~Calendar() = default;

public:
    /// @brief 获取日历实例
    /// @param type 日历类型
    /// @return
    static std::shared_ptr<Calendar> instance(CalendarType type);

    /// @brief
    /// 检查日期合法性，仅检查日期是否格式上合法，不关心实现方是否支持这个范围。
    /// @param dt
    /// @return
    static bool check_format_only(const DateTime& dt);

    /// @brief 判断年份是否时闰年
    /// @param year
    /// @return
    static bool is_leap(CSPT year);

    /// @brief 获取当前时间
    /// @return
    static DateTime now();

    /// @brief 对 value 取余数，结果一定大于 0。
    /// @param base
    /// @param value
    /// @return
    static CSPT mod(CSPT base, CSPT value);

    /// @brief 判断是否是同一天（不判断时间）
    /// @param d 日期一
    /// @param p 日期二
    /// @return
    static bool is_same_day(const DateTime& d, const DateTime& p);

    /// @brief 判断日期和时间是否一致
    /// @param d 日期一
    /// @param p 日期二
    /// @return
    static bool is_same_all(const DateTime& d, const DateTime& p);

    /// @brief 判断时间是否一致（不判断日期）
    /// @param d 日期一
    /// @param p 日期二
    /// @return
    static bool is_same_time(const DateTime& d, const DateTime& p);

public:
    /// @brief 实现方是否支持此日期
    /// @param dt
    /// @return
    virtual bool is_support(const DateTime& dt) = 0;

    /// @brief
    /// 设置某个时间点，并计算相关内容。一般最先调用这个，再使用其他函数。
    /// @param dt
    /// @return
    virtual bool generate(const DateTime& dt) = 0;

    /// @brief 获取公历日期
    /// @return
    virtual DateTime get_solar() const;

    /// @brief 获取农历日期
    /// @return
    virtual DateTime get_lunar() const;

    /// @brief 获取前 cnt 天的日期。
    /// @param dt
    /// @param cnt
    virtual void pre_day(DateTime& dt, unsigned int cnt) = 0;

    /// @brief 获取后 cnt 天的日期。
    /// @param dt
    /// @param cnt
    virtual void next_day(DateTime& dt, unsigned int cnt) = 0;

    /// @brief 获取两个时间之间的天数差，计算为 l - r。
    /// @param l
    /// @param r
    /// @return
    virtual CSPT days_difference(const DateTime& l, const DateTime& r) = 0;

    /// @brief 根据秒差值计算新日期。
    /// @param base
    /// @param secs 秒数
    /// @return
    virtual DateTime cal_new_date(const DateTime& base, CSPLT secs) = 0;

    /// @brief 获取距离 0点的秒数差。
    /// @param dt
    /// @return
    virtual CSPLT cal_zero_secs(const DateTime& dt) = 0;

    /// @brief 获取两个日期时间之间的秒数差（不记入天），计算为 l - r。
    /// @param l
    /// @param r
    /// @return
    virtual CSPT calsecs_by_time(const DateTime& l, const DateTime& r) = 0;

    /// @brief 返回两个时间之间的秒数差（记入天），计算为 l - r。
    /// @param l
    /// @param r
    /// @return
    virtual CSPLT calsecs_by_all(const DateTime& l, const DateTime& r) = 0;

    /// @brief 获取相邻的6个节气信息
    /// @return 
    std::array<JieQi, 6> get_jq_info() const;

public:
    /// @brief 获取干支信息
    /// @return
    GanZhi get_ganzhi() const;

protected:
    DateTime solar_{};      // 传入的时间日期
    DateTime lunar_{};      // 计算的农历日期
    bool leap_{false};      // 当月是否是闰月
    bool big_mon_{false};   // 当月是否是大月
    GanZhi ganzhi{};        // 此时的干支

    int wuhu_[10]{};    // 五虎遁年起月表
    int qishi_[10]{};   // 日上起时表

    std::array<JieQi, 6> jie_{};
};

}   // namespace csp
#endif