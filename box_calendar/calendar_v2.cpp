#include "calendar_v2.h"

#include <const.h>
#include <sxtwl.h>

namespace cppbox {

CCalenderV2::CCalenderV2() {}
CCalenderV2::~CCalenderV2() {}

bool CCalenderV2::setDateTime(const CDateTime& datetime) {
    m_datetime = datetime;
    Day* day =
        sxtwl::fromSolar(m_datetime.m_date.m_nYear, m_datetime.m_date.m_nMon,
                         m_datetime.m_date.m_nDay);
    m_ldatetime.m_date.m_nYear = day->getLunarYear();
    m_ldatetime.m_date.m_nMon = static_cast<int>(day->getLunarMonth());
    m_ldatetime.m_date.m_nDay = day->getLunarDay();
    return true;
}
// 获取前一天
void CCalenderV2::getPreDay(CDateTime& datetime) {}

void CCalenderV2::getPreDay(CDate& date) {}

// 获取后一天
void CCalenderV2::getNextDay(CDateTime& datetime) {}

void CCalenderV2::getNextDay(CDate& date) {}

// 检查日期格式是否正确
bool CCalenderV2::checkFormat(const CDateTime& datetime) { return true; }

// 返回两个日期之间的天数差
int CCalenderV2::getDiffByTwoDate(const CDate& dateA, const CDate& dateB) {
    return 0;
}

// 返回距离 00:00:00 的秒数
int CCalenderV2::getSecondsFromBase(const CTime& time) { return 0; }

// 基于基础时间和差值计算新的日期
void CCalenderV2::getDateTimeBySecond(const CDateTime& basetime,
                                      CDateTime& outtime, long long nSecond) {}

// 返回两个时间之间的秒数差
int CCalenderV2::getDiffByTwoTime(const CTime& timeA, const CTime& timeB) {
    return 0;
}

// 返回两个日期时间的秒数差
long long CCalenderV2::getSecondByTwoDateTime(const CDateTime& datetimeA,
                                              const CDateTime& datetimeB) {
    return 0;
}

}  // namespace cppbox