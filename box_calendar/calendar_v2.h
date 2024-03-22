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
    bool setDateTime(const CDateTime& datetime) override;
    // 获取前一天
    void getPreDay(CDateTime& datetime) override;
    void getPreDay(CDate& date) override;
    // 获取后一天
    void getNextDay(CDateTime& datetime) override;
    void getNextDay(CDate& date) override;
    // 检查日期格式是否正确
    bool checkFormat(const CDateTime& datetime) override;
    // 返回两个日期之间的天数差
    int getDiffByTwoDate(const CDate& dateA, const CDate& dateB) override;
    // 返回距离 00:00:00 的秒数
    int getSecondsFromBase(const CTime& time) override;
    // 基于基础时间和差值计算新的日期
    void getDateTimeBySecond(const CDateTime& basetime, CDateTime& outtime, long long nSecond) override;
    // 返回两个时间之间的秒数差
    int getDiffByTwoTime(const CTime& timeA, const CTime& timeB) override;
    // 返回两个日期时间的秒数差
    long long getSecondByTwoDateTime(const CDateTime& datetimeA, const CDateTime& datetimeB) override;

private:
    void sxtime2DateTime(Time& time, CDateTime& datetime);
    void sxday2DateTimeLunar(Day* day, CDate& date);
    void sxday2DateTimeSolar(Day* day, CDate& date);
    int  roundDouble(double number);

private:
    Day* day_{};
};

}   // namespace cppbox
#endif