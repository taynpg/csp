#include "calendar_v2.h"

namespace cppbox {

CCalenderV2::CCalenderV2() {}
CCalenderV2::~CCalenderV2() { delete day_; }

bool CCalenderV2::setDateTime(const CDateTime& datetime) {
    m_datetime = datetime;
    day_ =
        sxtwl::fromSolar(m_datetime.m_date.m_nYear, m_datetime.m_date.m_nMon,
                         m_datetime.m_date.m_nDay);

    m_ldatetime.m_time = m_datetime.m_time;
    sxday2DateTimeLunar(day_, m_ldatetime.m_date);

    m_sizhu.m_nYGan = day_->getYearGZ().tg;
    m_sizhu.m_nMGan = day_->getMonthGZ().tg;
    m_sizhu.m_nDGan = day_->getDayGZ().tg;

    m_sizhu.m_nYZhi = day_->getYearGZ().dz;
    m_sizhu.m_nMZhi = day_->getMonthGZ().dz;
    m_sizhu.m_nDZhi = day_->getDayGZ().dz;

    GZ& gz = sxtwl::getShiGz(m_sizhu.m_nDGan, m_datetime.m_time.m_nHour);
    m_sizhu.m_nHGan = gz.tg;
    m_sizhu.m_nHZhi = gz.dz;

    m_isLeap = day_->isLunarLeap();
    std::vector<sxtwl::JieQiInfo> jq_vec{};
    if (m_datetime.m_date.m_nMon == 1) {
        jq_vec = sxtwl::getJieQiByYear(m_datetime.m_date.m_nYear - 1);
    } else {
        jq_vec = sxtwl::getJieQiByYear(m_datetime.m_date.m_nYear);
    }

    std::vector<sxtwl::JieQiInfo>::const_iterator it;
    for (it = jq_vec.begin(); it != jq_vec.end(); ++it) {
        Time& time = sxtwl::JD2DD(it->jd);
        if (time.getMonth() != m_datetime.m_date.m_nMon) {
            continue;
        }
        if (time.getDay() < 15) {
            sxtime2DateTime(time, m_first.datetime);
            m_first.index = it->jqIndex;
        } else {
            sxtime2DateTime(time, m_second.datetime);
            m_second.index = it->jqIndex;
            break;
        }
    }
    return true;
}
// 获取前一天
void CCalenderV2::getPreDay(CDateTime& datetime) { getPreDay(datetime.m_date); }

void CCalenderV2::getPreDay(CDate& date)
{
    Day* tmp = day_;
    day_ = day_->before(1);
    sxday2DateTimeSolar(day_, date);
    delete tmp;
}

// 获取后一天
void CCalenderV2::getNextDay(CDateTime& datetime) 
{
    getNextDay(datetime.m_date);
}

void CCalenderV2::getNextDay(CDate& date) 
{
    Day* tmp = day_;
    day_ = day_->after(1);
    sxday2DateTimeSolar(day_, date);
    delete tmp;
}

// 检查日期格式是否正确
bool CCalenderV2::checkFormat(const CDateTime& datetime) { return true; }

// 返回两个日期之间的天数差
int CCalenderV2::getDiffByTwoDate(const CDate& dateA, const CDate& dateB) {
    Time ta(dateA.m_nYear, dateA.m_nMon, dateA.m_nDay, 0, 0, 0);
    Time tb(dateB.m_nYear, dateB.m_nMon, dateB.m_nDay, 0, 0, 0);
    double a = sxtwl::toJD(ta);
    double b = sxtwl::toJD(tb);
    double dd = a - b;
    return static_cast<int>(dd);
}

// 返回距离 00:00:00 的秒数
int CCalenderV2::getSecondsFromBase(const CTime& time) 
{
    return time.m_nHour * 3600 + time.m_nMin * 60 + time.m_nSec;
}

// 基于基础时间和差值计算新的日期
void CCalenderV2::getDateTimeBySecond(const CDateTime& basetime,
                                      CDateTime& outtime, long long nSecond) {}

// 返回两个时间之间的秒数差
int CCalenderV2::getDiffByTwoTime(const CTime& timeA, const CTime& timeB)
{
    int nBaseA = getSecondsFromBase(timeA);
    int nBaseB = getSecondsFromBase(timeB);
    return nBaseA - nBaseB;
}

// 返回两个日期时间的秒数差
long long CCalenderV2::getSecondByTwoDateTime(const CDateTime& datetimeA,
                                              const CDateTime& datetimeB) {
    Time   ta(datetimeA.m_date.m_nYear, datetimeA.m_date.m_nMon,
              datetimeA.m_date.m_nDay, datetimeA.m_time.m_nHour,
              datetimeA.m_time.m_nMin, datetimeA.m_time.m_nSec);
    Time   tb(datetimeB.m_date.m_nYear, datetimeB.m_date.m_nMon,
              datetimeB.m_date.m_nDay, datetimeB.m_time.m_nHour,
              datetimeB.m_time.m_nMin, datetimeB.m_time.m_nSec);
    double a = sxtwl::toJD(ta);
    double b = sxtwl::toJD(tb);
    double dd = a - b;
    return static_cast<int>(dd * 86400);
}

void CCalenderV2::sxtime2DateTime(Time& time, CDateTime& datetime)
{
    datetime.m_date.m_nYear = time.getYear();
    datetime.m_date.m_nMon = time.getMonth();
    datetime.m_date.m_nDay = time.getDay();
    datetime.m_time.m_nHour = static_cast<int>(time.getHour());
    datetime.m_time.m_nMin = static_cast<int>(time.getMin());
    datetime.m_time.m_nSec = roundDouble(time.getSec());
}

void CCalenderV2::sxday2DateTimeLunar(Day* day, CDate& date) 
{
    date.m_nYear = day_->getLunarYear();
    date.m_nMon = static_cast<int>(day_->getLunarMonth());
    date.m_nDay = day_->getLunarDay();
}

void CCalenderV2::sxday2DateTimeSolar(Day* day, CDate& date) {
    date.m_nYear = day_->getSolarYear();
    date.m_nMon = static_cast<int>(day_->getSolarMonth());
    date.m_nDay = day_->getSolarDay();
}

int CCalenderV2::roundDouble(double number)
{ 
    double ret = (number > 0.0) ? (number + 0.5) : (number - 0.5);
    return static_cast<int>(ret);
}

}  // namespace cppbox