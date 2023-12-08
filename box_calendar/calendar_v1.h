#ifndef BOX_CALENDAR_HEADER_
#define BOX_CALENDAR_HEADER_

#include "calendar_base.h"

namespace cppbox {
constexpr int g_cnum = 10;
using cb = CCalenderBase;
// 日期处理类
class CCalender : public CCalenderBase {
public:
    CCalender();
    ~CCalender() override;

public:
    // 设置日期并计算，返回是否成功。
    bool setDateTime(const CDateTime& datetime) override;

private:
    // 初始化数据
    void initSelf();
    // 检查日期格式是否正确
    bool checkFormat(const CDateTime& datetime) override;

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
    // 基于基础时间和差值计算新的日期
    static void getDateTimeBySecond(const CDateTime& basetime,
                                    CDateTime& outtime, long long nSecond);
    // 返回公历日期的前一天日期
    static void getPreDay(const CDateTime& datetime, CDateTime& outtime);
    void        getPreDay(CDateTime& datetime) override;
    void        getPreDay(CDate& date) override;
    void        getNextDay(CDateTime& datetime) override;
    void        getNextDay(CDate& date) override;
    // 获取某年第 x 个节气为几号(从 0 小寒算起)
    int getLunarSterm(int nYear, int nth, CDateTime& datetime);
    // 复制日期
    static void copyDateTime(const CDateTime& datetime, CDateTime& outtime);
    // 计算农历的年干支，月干支 (立春为新年)
    void getYMGanZhi();
    // 计算时辰的干支(在计算完年月干支后进行)
    void getHourGanZhi();

private:
    // 返回下一个月的年干支月干支信息
    void getNextJie(int& nYear, int& nMon, int& nJie, int& nMonJiaZi,
                    int& nYearJiaZi);
    // 获取上一个月的年月干支，最后一个参数为当月是否是 立春
    void getPreMonYMGanZhi(int nJiaZiY, int nJiaZiM, int& nJiaZiYOut,
                           int& nJiaZiMOut, bool isLiChun);
    // 给定年干支，返回下一年的年干支和起始月干支
    void getNextYMGanZhi(int nJiaZiY, int& nJiaZiNextY, int& nJiaZiNextM);

private:
    int* m_pJieSum{};  // 计算节气的差值数组
    int* m_pLunar{};   // 农历表数组
private:
    // 辅助表
    int m_wuhu[g_cnum]{};   // 五虎遁年起月表
    int m_qishi[g_cnum]{};  // 日上起时表
};
}  // namespace cppbox
#endif
