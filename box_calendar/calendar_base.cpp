#include "calendar_base.h"

#include "calendar_v1.h"
#include "calendar_v2.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <ctime>
#endif

namespace cppbox {

bool CDate::operator==(const CDate& rh) const
{
    if (m_nYear == rh.m_nYear && m_nMon == rh.m_nMon && m_nDay == rh.m_nDay) {
        return true;
    }
    return false;
}

CDate::CDate() = default;
CDate::CDate(const CDate& date)
{
    m_nYear = date.m_nYear;
    m_nMon = date.m_nMon;
    m_nDay = date.m_nDay;
}
CDate::CDate(int y, int m, int d)
{
    m_nYear = y;
    m_nMon = m;
    m_nDay = d;
}
CDate& CDate::operator=(const CDate& date)
{
    if (this == &date) {
        return *this;
    }
    this->m_nDay = date.m_nDay;
    this->m_nMon = date.m_nMon;
    this->m_nYear = date.m_nYear;
    return *this;
}

bool CDate::operator!=(const CDate& date)
{
    if (this->m_nYear != date.m_nYear) {
        return true;
    }
    if (this->m_nMon != date.m_nMon) {
        return true;
    }
    if (this->m_nDay != date.m_nDay) {
        return true;
    }
    return false;
}

CDateTime::CDateTime(const CDate& rh)
{
    m_date.m_nYear = rh.m_nYear;
    m_date.m_nMon = rh.m_nMon;
    m_date.m_nDay = rh.m_nDay;
    m_time.m_nHour = 12;
    m_time.m_nMin = 0;
    m_time.m_nSec = 0;
}

CDateTime::CDateTime(int y, int m, int d, int h, int min, int sec)
{
    m_date.m_nYear = y;
    m_date.m_nMon = m;
    m_date.m_nDay = d;
    m_time.m_nHour = h;
    m_time.m_nMin = min;
    m_time.m_nSec = sec;
}

CDateTime& CDateTime::operator=(const CDateTime& datetime)
{
    if (this == &datetime) {
        return *this;
    }
    this->m_date.m_nYear = datetime.m_date.m_nYear;
    this->m_date.m_nMon = datetime.m_date.m_nMon;
    this->m_date.m_nDay = datetime.m_date.m_nDay;
    this->m_time.m_nHour = datetime.m_time.m_nHour;
    this->m_time.m_nMin = datetime.m_time.m_nMin;
    this->m_time.m_nSec = datetime.m_time.m_nSec;
    return *this;
}

CDateTime& CDateTime::operator=(const CDate& date)
{
    this->m_date.m_nYear = date.m_nYear;
    this->m_date.m_nMon = date.m_nMon;
    this->m_date.m_nDay = date.m_nDay;
    return *this;
}

CCalenderBase* CCalenderFactory::creatInstance(CalendarType etype)
{
    CCalenderBase* pResult = nullptr;

    switch (etype) {
        case CALENDAR_V1: {
            pResult = new CCalenderV1();
            break;
        }
        case CALENDAR_V2: {
            pResult = new CCalenderV2();
            break;
        }
        default:
            break;
    }
    return pResult;
}

// 释放内存
void CCalenderFactory::freeCalender(CCalenderBase* pCalender)
{
    delete pCalender;
}

CCalenderBase::CCalenderBase() = default;

CDateTime const& CCalenderBase::getDateTime() const { return m_datetime; }

CDateTime const& CCalenderBase::getLunarDateTime() const { return m_ldatetime; }
#ifdef _WIN32
// 获取系统时间
void CCalenderBase::getNowDateTime(CDateTime& datetime)
{
    SYSTEMTIME nowTime;
    GetLocalTime(&nowTime);
    datetime.m_date.m_nYear = nowTime.wYear;
    datetime.m_date.m_nMon = nowTime.wMonth;
    datetime.m_date.m_nDay = nowTime.wDay;
    datetime.m_time.m_nHour = nowTime.wHour;
    datetime.m_time.m_nMin = nowTime.wMinute;
    datetime.m_time.m_nSec = nowTime.wSecond;
}
#else
// 获取系统时间
void CCalenderBase::getNowDateTime(CDateTime& datetime)
{
    struct std::tm* nowTime = nullptr;
    std::time_t     _otime_t;
    std::time(&_otime_t);
    nowTime = std::localtime(&_otime_t);
    datetime.m_date.m_nYear = nowTime->tm_year + 1900;
    datetime.m_date.m_nMon = nowTime->tm_mon + 1;
    datetime.m_date.m_nDay = nowTime->tm_mday;
    datetime.m_time.m_nHour = nowTime->tm_hour;
    datetime.m_time.m_nMin = nowTime->tm_min;
    datetime.m_time.m_nSec = nowTime->tm_sec;
}
#endif

// 仅检查日期格式上的合法性，并不考虑实现方是否支持这个日期范围
bool CCalenderBase::checkFormatOnly(const CDateTime& datetime)
{
    if (datetime.m_date.m_nMon < 1 || datetime.m_date.m_nMon > 12) {
        return false;
    }

    int y = datetime.m_date.m_nYear;
    int m = datetime.m_date.m_nMon;
    int d = datetime.m_date.m_nDay;

    if (d < 1) {
        return false;
    }

    if (CCalenderBase::isLeapYear(y)) {
        if (m == 2) {
            if (d > 29) {
                return false;
            }
        }
    } else {
        if (m == 2) {
            if (d > 28) {
                return false;
            }
        }
    }

    if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) {
        if (d > 31) {
            return false;
        }
    }

    if (m == 4 || m == 6 || m == 9 || m == 11) {
        if (d > 30) {
            return false;
        }
    }

    int h = datetime.m_time.m_nHour;
    int min = datetime.m_time.m_nMin;
    int s = datetime.m_time.m_nSec;

    if (h > 23 || h < 0) {
        return false;
    }
    if (min > 59 || min < 0) {
        return false;
    }
    if (s > 59 || s < 0) {
        return false;
    }

    return true;
}

bool CCalenderBase::isLeapYear(int year)
{
    if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)) {
        return true;
    }
    return false;
}

// 求余数(结果大于0)
int CCalenderBase::getRemainder(int nBase, int nValue)
{
    int nRet = nValue % nBase;
    if (nRet < 0) {
        nRet += nBase;
    }
    return nRet;
}

// 复制日期
void CCalenderBase::copyDateTime(const CDateTime& datetime, CDateTime& outtime)
{
    outtime.m_date.m_nYear = datetime.m_date.m_nYear;
    outtime.m_date.m_nMon = datetime.m_date.m_nMon;
    outtime.m_date.m_nDay = datetime.m_date.m_nDay;
    outtime.m_time.m_nHour = datetime.m_time.m_nHour;
    outtime.m_time.m_nMin = datetime.m_time.m_nMin;
    outtime.m_time.m_nSec = datetime.m_time.m_nSec;
}

// 获取四柱
CGanZhi const& CCalenderBase::getSizhu() const { return m_sizhu; }

// 获取第一个节气
CJieQi const& CCalenderBase::getJieFirst() const { return m_first; }
// 获取第二个节气
CJieQi const& CCalenderBase::getJieSecond() const { return m_second; }

CGanZhi& CGanZhi::operator=(const CGanZhi& ganzhi)
{
    if (this == &ganzhi) {
        return *this;
    }
    this->m_nDGan = ganzhi.m_nDGan;
    this->m_nDZhi = ganzhi.m_nDZhi;
    this->m_nHGan = ganzhi.m_nHGan;
    this->m_nHZhi = ganzhi.m_nHZhi;
    this->m_nMGan = ganzhi.m_nMGan;
    this->m_nMZhi = ganzhi.m_nMZhi;
    this->m_nYGan = ganzhi.m_nYGan;
    this->m_nYZhi = ganzhi.m_nYZhi;
    return *this;
}
}  // namespace cppbox
