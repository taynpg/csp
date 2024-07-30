#ifndef BOX_CALENDAR_HEADER_V1
#define BOX_CALENDAR_HEADER_V1

#include "calendar_base.h"

namespace cppbox {
constexpr int g_cnum = 10;
using cb = CCalenderBase;
// 日期处理类
class CCalenderV1 : public CCalenderBase
{
public:
    CCalenderV1();
    ~CCalenderV1() override;

public:
    // 设置日期并计算，返回是否成功。
    bool set_datetime(const CDateTime& datetime) override;
    // 检查日期格式是否正确
    bool check_format(const CDateTime& datetime) override;
    // 获取前一天
    void pre(CDateTime& datetime) override;
    void pre(CDate& date) override;
    // 获取后一天
    void next(CDateTime& datetime) override;
    void next(CDate& date) override;
    // 返回两个日期之间的天数差
    int get_diff_day(const CDate& dateA, const CDate& dateB) override;

    // 基于基础时间和差值计算新的日期
    void get_diff_sec(const CDateTime& basetime, CDateTime& outtime, long long nSecond) override;

    // 返回距离 00:00:00 的秒数
    int get_sec_by_base(const CTime& time) override;

    // 返回两个时间之间的秒数差
    int get_diff_by_time(const CTime& timeA, const CTime& timeB) override;

    // 返回两个日期时间的秒数差
    long long get_sec_by_date(const CDateTime& datetimeA, const CDateTime& datetimeB) override;

private:
    // 初始化数据
    void initSelf();

private:
    // 农历指定年所闰的月数，没有返回0
    int getLeapMon(int nYear);
    // 返回农历 xx 年闰月的天数
    int getLeapDays(int nYear);
    // 返回农历 xx 年的总的天数
    int getLunarDays(int nYear);
    // 返回农历 xx 年 xx 月的天数
    int getLunarDay(int nYear, int nMon);

private:
    // 返回公历日期的前一天日期
    static void getPreDay(const CDateTime& datetime, CDateTime& outtime);

    // 获取某年第 x 个节气为几号(从 0 小寒算起)
    int getLunarSterm(int nYear, int nth, CDateTime& datetime);

    // 计算农历的年干支，月干支 (立春为新年)
    void getYMGanZhi();
    // 计算时辰的干支(在计算完年月干支后进行)
    void getHourGanZhi();

private:
    // 返回公历日期的后一天日期
    static void getNextDay(const CDateTime& datetime, CDateTime& outtime);

    // 返回距离 1900.1.1 的天数
    static int getDaysFromBase(const CDate& date);

    // 返回下一个月的年干支月干支信息
    void getNextJie(int& nYear, int& nMon, int& nJie, int& nMonJiaZi, int& nYearJiaZi);
    // 获取上一个月的年月干支，最后一个参数为当月是否是 立春
    void getPreMonYMGanZhi(int nJiaZiY, int nJiaZiM, int& nJiaZiYOut, int& nJiaZiMOut, bool isLiChun);
    // 给定年干支，返回下一年的年干支和起始月干支
    void getNextYMGanZhi(int nJiaZiY, int& nJiaZiNextY, int& nJiaZiNextM);

private:
    int* m_pJieSum{};   // 计算节气的差值数组
    int* m_pLunar{};    // 农历表数组
private:
    // 辅助表
    int m_wuhu[g_cnum]{};    // 五虎遁年起月表
    int m_qishi[g_cnum]{};   // 日上起时表
};
}   // namespace cppbox
#endif
