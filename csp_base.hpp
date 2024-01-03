#ifndef CSP_HEADER_H
#define CSP_HEADER_H

#define CSP_VERSION "v1.3.0_Pre1"

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#endif
#include <string>

struct CMDParam
{
    int year{};
    int mon{};
    int day{};
    int hour{};
    int min{};
    int sec{};
    int nJu{};
    int nType = -1;
    std::string datetime_{};
};

inline void FillTime(CMDParam& param)
{
#ifdef _WIN32
// 获取系统时间
    SYSTEMTIME time;
    GetLocalTime(&time);
    param.year = time.wYear;
    param.mon = time.wMonth;
    param.day = time.wDay;
    param.hour = time.wHour;
    param.min = time.wMinute;
    param.sec = time.wSecond;
#else
// 获取系统时间
    struct tm* nowtime = NULL;
    time_t t;
    time(&t);
    nowtime = localtime(&t);
    param.year = nowtime->tm_year + 1900; 
    param.mon = nowtime->tm_mon + 1;
    param.day = nowtime->tm_mday;
    param.hour = nowtime->tm_hour;
    param.min = nowtime->tm_min;
    param.sec = nowtime->tm_sec;
#endif
}

#endif
