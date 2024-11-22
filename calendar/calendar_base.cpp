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
    if (year_ == rh.year_ && mon_ == rh.mon_ && day_ == rh.day_) {
        return true;
    }
    return false;
}

CDate::CDate() = default;
CDate::CDate(const CDate& date)
{
    year_ = date.year_;
    mon_ = date.mon_;
    day_ = date.day_;
}
CDate::CDate(int y, int m, int d)
{
    year_ = y;
    mon_ = m;
    day_ = d;
}
CDate& CDate::operator=(const CDate& date)
{
    if (this == &date) {
        return *this;
    }
    this->day_ = date.day_;
    this->mon_ = date.mon_;
    this->year_ = date.year_;
    return *this;
}

bool CDate::operator!=(const CDate& date)
{
    if (this->year_ != date.year_) {
        return true;
    }
    if (this->mon_ != date.mon_) {
        return true;
    }
    if (this->day_ != date.day_) {
        return true;
    }
    return false;
}

CDateTime::CDateTime(const CDate& rh)
{
    date_.year_ = rh.year_;
    date_.mon_ = rh.mon_;
    date_.day_ = rh.day_;
    time_.h_ = 12;
    time_.m_ = 0;
    time_.s_ = 0;
}

CDateTime::CDateTime(int y, int m, int d, int h, int min, int sec)
{
    date_.year_ = y;
    date_.mon_ = m;
    date_.day_ = d;
    time_.h_ = h;
    time_.m_ = min;
    time_.s_ = sec;
}

CDateTime& CDateTime::operator=(const CDateTime& datetime)
{
    if (this == &datetime) {
        return *this;
    }
    this->date_.year_ = datetime.date_.year_;
    this->date_.mon_ = datetime.date_.mon_;
    this->date_.day_ = datetime.date_.day_;
    this->time_.h_ = datetime.time_.h_;
    this->time_.m_ = datetime.time_.m_;
    this->time_.s_ = datetime.time_.s_;
    return *this;
}

CDateTime& CDateTime::operator=(const CDate& date)
{
    this->date_.year_ = date.year_;
    this->date_.mon_ = date.mon_;
    this->date_.day_ = date.day_;
    return *this;
}

CCalender* CCalenderFactory::creat_instance(CalendarType etype)
{
    CCalender* pResult = nullptr;

    switch (etype) {
        case CalendarType::CALENDAR_V1: {
            pResult = new CCalenderV1();
            break;
        }
        case CalendarType::CALENDAR_V2: {
            pResult = new CCalenderV2();
            break;
        }
        default:
            break;
    }
    return pResult;
}

// 释放内存
void CCalenderFactory::free(CCalender* pCalender)
{
    delete pCalender;
}

CCalender::CCalender() = default;

CDateTime const& CCalender::get_solar() const
{
    return dt_;
}

CDateTime const& CCalender::get_lunnar() const
{
    return lunar_;
}
#ifdef _WIN32
// 获取系统时间
void CCalender::now(CDateTime& datetime)
{
    SYSTEMTIME nowTime;
    GetLocalTime(&nowTime);
    datetime.date_.year_ = nowTime.wYear;
    datetime.date_.mon_ = nowTime.wMonth;
    datetime.date_.day_ = nowTime.wDay;
    datetime.time_.h_ = nowTime.wHour;
    datetime.time_.m_ = nowTime.wMinute;
    datetime.time_.s_ = nowTime.wSecond;
}
#else
// 获取系统时间
void CCalender::now(CDateTime& datetime)
{
    struct std::tm* nowTime = nullptr;
    std::time_t _otime_t;
    std::time(&_otime_t);
    nowTime = std::localtime(&_otime_t);
    datetime.date_.year_ = nowTime->tm_year + 1900;
    datetime.date_.mon_ = nowTime->tm_mon + 1;
    datetime.date_.day_ = nowTime->tm_mday;
    datetime.time_.h_ = nowTime->tm_hour;
    datetime.time_.m_ = nowTime->tm_min;
    datetime.time_.s_ = nowTime->tm_sec;
}
#endif

// 仅检查日期格式上的合法性，并不考虑实现方是否支持这个日期范围
bool CCalender::check_format_only(const CDateTime& datetime)
{
    if (datetime.date_.mon_ < 1 || datetime.date_.mon_ > 12) {
        return false;
    }

    int y = datetime.date_.year_;
    int m = datetime.date_.mon_;
    int d = datetime.date_.day_;

    if (d < 1) {
        return false;
    }

    if (CCalender::is_leap(y)) {
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

    int h = datetime.time_.h_;
    int min = datetime.time_.m_;
    int s = datetime.time_.s_;

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

bool CCalender::is_leap(int year)
{
    if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)) {
        return true;
    }
    return false;
}

// 求余数(结果大于0)
int CCalender::remain(int nBase, int nValue)
{
    int nRet = nValue % nBase;
    if (nRet < 0) {
        nRet += nBase;
    }
    return nRet;
}

// 复制日期
void CCalender::mcopy(const CDateTime& datetime, CDateTime& outtime)
{
    outtime.date_.year_ = datetime.date_.year_;
    outtime.date_.mon_ = datetime.date_.mon_;
    outtime.date_.day_ = datetime.date_.day_;
    outtime.time_.h_ = datetime.time_.h_;
    outtime.time_.m_ = datetime.time_.m_;
    outtime.time_.s_ = datetime.time_.s_;
}

// 获取四柱
CGanZhi const& CCalender::get_sz() const
{
    return sz_;
}

CJieQi6 CCalender::get_jie() const
{
    return jie_;
}

CGanZhi& CGanZhi::operator=(const CGanZhi& ganzhi)
{
    if (this == &ganzhi) {
        return *this;
    }
    this->dg_ = ganzhi.dg_;
    this->dz_ = ganzhi.dz_;
    this->hg_ = ganzhi.hg_;
    this->hz_ = ganzhi.hz_;
    this->mg_ = ganzhi.mg_;
    this->mz_ = ganzhi.mz_;
    this->yg_ = ganzhi.yg_;
    this->yz_ = ganzhi.yz_;
    return *this;
}
}   // namespace cppbox
