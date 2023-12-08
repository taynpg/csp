#include "calendar_base.h"

#include "calendar_v1.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <ctime>
#endif

namespace cppbox {

bool CDate::operator==(const CDate& rh) const {
    if (m_nYear == rh.m_nYear && m_nMon == rh.m_nMon && m_nDay == rh.m_nDay) {
        return true;
    }
    return false;
}

CDate::CDate() = default;
CDate::CDate(const CDate& date) {
    m_nYear = date.m_nYear;
    m_nMon = date.m_nMon;
    m_nDay = date.m_nDay;
}
CDate::CDate(int y, int m, int d) {
    m_nYear = y;
    m_nMon = m;
    m_nDay = d;
}
CDate& CDate::operator=(const CDate& date) {
    if (this == &date) {
        return *this;
    }
    this->m_nDay = date.m_nDay;
    this->m_nMon = date.m_nMon;
    this->m_nYear = date.m_nYear;
    return *this;
}

CDateTime::CDateTime(const CDate& rh) {
    m_date.m_nYear = rh.m_nYear;
    m_date.m_nMon = rh.m_nMon;
    m_date.m_nDay = rh.m_nDay;
    m_time.m_nHour = 12;
    m_time.m_nMin = 0;
    m_time.m_nSec = 0;
}

CDateTime::CDateTime(int y, int m, int d, int h, int min, int sec) {
    m_date.m_nYear = y;
    m_date.m_nMon = m;
    m_date.m_nDay = d;
    m_time.m_nHour = h;
    m_time.m_nMin = min;
    m_time.m_nSec = sec;
}

CDateTime& CDateTime::operator=(const CDateTime& datetime) {
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

CCalenderBase* CCalenderFactory::creatInstance(CalendarType etype) {
    CCalenderBase* pResult = nullptr;

    switch (etype) {
        case CALENDAR_V1: {
            pResult = new CCalender();
            break;
        }
        default:
            break;
    }
    return pResult;
}

// 释放内存
void CCalenderFactory::freeCalender(CCalenderBase* pCalender) {
    delete pCalender;
}

CCalenderBase::CCalenderBase() = default;

CDateTime const& CCalenderBase::getDateTime() const { return m_datetime; }

CDateTime const& CCalenderBase::getLunarDateTime() const { return m_ldatetime; }
#ifdef _WIN32
// 获取系统时间
void CCalenderBase::getNowDateTime(CDateTime& datetime) {
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
void CCalenderBase::getNowDateTime(CDateTime& datetime) {
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

// 求余数(结果大于0)
int CCalenderBase::getRemainder(int nBase, int nValue) {
    int nRet = nValue % nBase;
    if (nRet < 0) {
        nRet += nBase;
    }
    return nRet;
}

// 返回距离 00:00:00 的秒数
int CCalenderBase::getSecondsFromBase(const CTime& time) {
    return time.m_nHour * 3600 + time.m_nMin * 60 + time.m_nSec;
}

// 返回两个日期之间的天数差
int CCalenderBase::getDiffByTwoDate(const CDate& dateA, const CDate& dateB) {
    int nBaseA = getDaysFromBase(dateA);
    int nBaseB = getDaysFromBase(dateB);
    return nBaseA - nBaseB;
}

// 返回两个时间之间的秒数差
int CCalenderBase::getDiffByTwoTime(const CTime& timeA, const CTime& timeB) {
    int nBaseA = getSecondsFromBase(timeA);
    int nBaseB = getSecondsFromBase(timeB);
    return nBaseA - nBaseB;
}

// 返回两个日期时间的秒数差
long long CCalenderBase::getSecondByTwoDateTime(const CDateTime& datetimeA,
                                                const CDateTime& datetimeB) {
    long long nBaseA = getDiffByTwoTime(datetimeA.m_time, datetimeB.m_time);
    long long nBaseB = getDiffByTwoDate(datetimeA.m_date, datetimeB.m_date);
    return (nBaseA + nBaseB * 86400);
}

int CCalenderBase::getDaysFromBase(const CDate& date) {
    int y = date.m_nYear;
    int m = date.m_nMon;
    int d = date.m_nDay;

    int ny = 0, nm = 0, nd = 0, nsum = 0;

    if (y >= 1900) {
        for (int i = 1900; i < y; ++i) {
            if (isLeap(i)) {
                ny += 366;
            } else {
                ny += 365;
            }
        }
        for (int i = 1; i < m; ++i) {
            switch (i) {
                case 1:
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                case 12:
                    nm += 31;
                    break;
                case 4:
                case 6:
                case 9:
                case 11:
                    nm += 30;
                    break;
                case 2: {
                    if (isLeap(y)) {
                        nm += 29;
                    } else {
                        nm += 28;
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        }
        nd = d - 1;
        nsum = ny + nm + nd;
    } else {
        for (int i = y; i < 1900; ++i) {
            if (isLeap(i)) {
                ny += 366;
            } else {
                ny += 365;
            }
        }
        for (int i = 1; i < m; ++i) {
            switch (i) {
                case 1:
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                case 12:
                    nm += 31;
                    break;
                case 4:
                case 6:
                case 9:
                case 11:
                    nm += 30;
                    break;
                case 2: {
                    if (isLeap(y)) {
                        nm += 29;
                    } else {
                        nm += 28;
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        }
        nd = d - 1;
        nsum = ny - nm - nd;
        nsum = -nsum;
    }
    return nsum;
}

// 返回公历日期的后一天日期
void CCalenderBase::getNextDay(const CDateTime& datetime, CDateTime& outtime) {
    int y = datetime.m_date.m_nYear;
    int m = datetime.m_date.m_nMon;
    int d = datetime.m_date.m_nDay;

    outtime.m_time.m_nHour = datetime.m_time.m_nHour;
    outtime.m_time.m_nMin = datetime.m_time.m_nMin;
    outtime.m_time.m_nSec = datetime.m_time.m_nSec;

    int nDay = 0;
    switch (m) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12: {
            nDay = 31;
            break;
        }
        case 2: {
            if (isLeap(y)) {
                nDay = 29;
            } else {
                nDay = 28;
            }
            break;
        }
        default: {
            nDay = 30;
            break;
        }
    }

    if (m <= 11) {
        outtime.m_date.m_nYear = y;
        if (nDay == d) {
            outtime.m_date.m_nMon = m + 1;
            outtime.m_date.m_nDay = 1;
        } else {
            outtime.m_date.m_nMon = m;
            outtime.m_date.m_nDay = d + 1;
        }
    } else if (m == 12) {
        if (d == nDay) {
            outtime.m_date.m_nYear = y + 1;
            outtime.m_date.m_nMon = 1;
            outtime.m_date.m_nDay = 1;
        } else {
            outtime.m_date.m_nYear = y;
            outtime.m_date.m_nMon = 12;
            outtime.m_date.m_nDay = d + 1;
        }
    }
}

bool CCalenderBase::isLeap(int nYear) {
    if (((nYear % 4 == 0) && (nYear % 100 != 0)) || (nYear % 400 == 0)) {
        return true;
    }
    return false;
}

// 获取四柱
CGanZhi const& CCalenderBase::getSizhu() const { return m_sizhu; }

// 获取第一个节气
CJieQi const& CCalenderBase::getJieFirst() const { return m_first; }
// 获取第二个节气
CJieQi const& CCalenderBase::getJieSecond() const { return m_second; }

CGanZhi& CGanZhi::operator=(const CGanZhi& ganzhi) {
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
