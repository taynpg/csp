#include "calendar_v1.h"

#include "calendar_v1_data.h"

namespace cppbox {

CCalenderV1::CCalenderV1() { initSelf(); }

CCalenderV1::~CCalenderV1() {
    FreeIntList(m_pJieSum);
    FreeIntList(m_pLunar);
}

void CCalenderV1::initSelf() {
    if (!AllocJieQi(&m_pJieSum)) {
        return;
    }
    if (!AllocLunar(&m_pLunar)) {
        return;
    }
    m_isLeap = false;
    m_bigMon = false;

    m_wuhu[0] = 2;
    m_wuhu[1] = 4;
    m_wuhu[2] = 6;
    m_wuhu[3] = 8;
    m_wuhu[4] = 0;
    m_wuhu[5] = 2;
    m_wuhu[6] = 4;
    m_wuhu[7] = 6;
    m_wuhu[8] = 8;
    m_wuhu[9] = 0;

    m_qishi[0] = 0;
    m_qishi[1] = 2;
    m_qishi[2] = 4;
    m_qishi[3] = 6;
    m_qishi[4] = 8;
    m_qishi[5] = 0;
    m_qishi[6] = 2;
    m_qishi[7] = 4;
    m_qishi[8] = 6;
    m_qishi[9] = 8;
}

bool CCalenderV1::setDateTime(const CDateTime& datetime) {
    // 检查日期格式是否合法
    if (!checkFormat(datetime)) {
        return false;
    }

    m_ldatetime = datetime;
    copyDateTime(datetime, m_datetime);
    CDate sdate;
    sdate.m_nYear = 1900;
    sdate.m_nMon = 1;
    sdate.m_nDay = 31;
    int offset = getDiffByTwoDate(datetime.m_date, sdate);
    // int offset = getDaysFromBase(datetime.m_date);
    int i = 0, nleap = 0, ntem = 0;
    for (i = 1900; i < 2100 && offset > 0; ++i) {
        ntem = getLunarDays(i);
        offset -= ntem;
    }
    if (offset < 0) {
        offset += ntem;
        --i;
    }

    m_ldatetime.m_date.m_nYear = i;
    nleap = getLeapMon(i);
    m_isLeap = false;

    for (i = 1; i < 13 && offset > 0; ++i) {
        // 闰月
        if (nleap > 0 && i == (nleap + 1) && (!m_isLeap)) {
            --i;
            m_isLeap = true;
            ntem = getLeapDays(m_ldatetime.m_date.m_nYear);
        } else {
            ntem = getLunarDay(m_ldatetime.m_date.m_nYear, i);
        }
        // 解除闰月
        if (m_isLeap && i == (nleap + 1)) {
            m_isLeap = false;
        }
        offset -= ntem;
    }

    if (offset == 0 && nleap > 0 && i == nleap + 1) {
        if (m_isLeap) {
            m_isLeap = false;
        } else {
            m_isLeap = true;
            --i;
        }
    }

    if (offset < 0) {
        offset += ntem;
        --i;
    }

    m_ldatetime.m_date.m_nMon = i;
    m_ldatetime.m_date.m_nDay = offset + 1;
    m_bigMon = m_isLeap ? getLeapDays(m_ldatetime.m_date.m_nYear) != 0
                        : getLunarDay(m_ldatetime.m_date.m_nYear,
                                      m_ldatetime.m_date.m_nMon) > 29;

    // 计算节气相关
    getLunarSterm(datetime.m_date.m_nYear, (datetime.m_date.m_nMon - 1) * 2,
                  m_first.datetime);
    m_first.index = (m_first.datetime.m_date.m_nMon - 1) * 2;

    getLunarSterm(datetime.m_date.m_nYear, (datetime.m_date.m_nMon - 1) * 2 + 1,
                  m_second.datetime);
    m_second.index = (m_second.datetime.m_date.m_nMon - 1) * 2 + 1;

    // 计算日柱
    int nDayCnt = getDaysFromBase(datetime.m_date);
    if (datetime.m_time.m_nHour == 23) {
        ++nDayCnt;
    }
    m_sizhu.m_nDGan = nDayCnt % 10;
    m_sizhu.m_nDZhi = (nDayCnt + 10) % 12;

    getYMGanZhi();
    getHourGanZhi();

    return true;
}

bool CCalenderV1::checkFormat(const CDateTime& datetime) {
    if (datetime.m_date.m_nYear < 1901 || datetime.m_date.m_nYear > 2099) {
        return false;
    }
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

// 农历指定年所闰的月数，没有返回0
int CCalenderV1::getLeapMon(int nYear) {
    int nRet = m_pLunar[nYear - 1900] & 0xf;
    if (nRet == 0xf) {
        return 0;
    }
    return nRet;
}

// 返回农历 xx 年闰月的天数
int CCalenderV1::getLeapDays(int nYear) {
    if (getLeapMon(nYear)) {
        int nRet = m_pLunar[nYear - 1899] & 0xf;
        if (nRet == 0xf) {
            return 30;
        }
        return 29;
    }
    return 0;
}

// 返回农历 xx 年的总的天数
int CCalenderV1::getLunarDays(int nYear) {
    int i = 0, nsum = 348, ntem = 0;
    for (i = 0x8000; i > 0x8; i >>= 1) {
        ntem = m_pLunar[nYear - 1900];
        nsum += (ntem & i) ? 1 : 0;
    }
    return nsum + getLeapDays(nYear);
}

// 返回农历 xx 年 xx 月的天数
int CCalenderV1::getLunarDay(int nYear, int nMon) {
    int nRe = m_pLunar[nYear - 1900] & (0x10000 >> nMon);
    if (nRe) {
        return 30;
    }
    return 29;
}

// 计算时辰的干支(在计算完年月干支后进行)
void CCalenderV1::getHourGanZhi() {
    int nStart = m_qishi[m_sizhu.m_nDGan];
    int nStartGan = cb::getRemainder(10, nStart) * 6;
    int nhour = m_datetime.m_time.m_nHour + 1;
    if (nhour == 24) {
        nhour = 0;
    }
    m_sizhu.m_nHGan = cb::getRemainder(10, nStartGan + nhour / 2);
    m_sizhu.m_nHZhi = nhour / 2;
}

// 计算农历的年干支，月干支 (立春为新年)
void CCalenderV1::getYMGanZhi() {
    // 以公历 1900 年 01 月 6 日时为起始日期(小寒）对应农历日期为 1900 年
    // 己亥年，丁丑月 (以立春为新年)
    int nStartYGanZhi = 35;  // 己亥 的索引
    int nStartMGanZhi = 13;  // 丁丑 的索引　
    int nJieQiIndex = 0;     // 小寒 的索引
    int nStartYear = 1900;
    int nStartMon = 1;

    int nJieQiCnt = (m_datetime.m_date.m_nYear - 1900) * 24 + m_first.index;
    for (int i = 0; i < nJieQiCnt; ++i) {
        getNextJie(nStartYear, nStartMon, nJieQiIndex, nStartMGanZhi,
                   nStartYGanZhi);
    }
    long long nChange = getSecondByTwoDateTime(m_first.datetime, m_datetime);
    int       nTemA = 0, nTemB = 0;
    if (nChange < 0) {
        nTemA = nStartYGanZhi;
        nTemB = nStartMGanZhi;
    } else {
        if (m_first.index == 2) {
            getPreMonYMGanZhi(nStartYGanZhi, nStartMGanZhi, nTemA, nTemB, true);
        } else {
            getPreMonYMGanZhi(nStartYGanZhi, nStartMGanZhi, nTemA, nTemB,
                              false);
        }
    }
    m_sizhu.m_nYGan = nTemA % 10;
    m_sizhu.m_nYZhi = nTemA % 12;
    m_sizhu.m_nMGan = nTemB % 10;
    m_sizhu.m_nMZhi = nTemB % 12;
}
// 返回下一个月的年干支月干支信息
void CCalenderV1::getNextJie(int& nYear, int& nMon, int& nJie, int& nMonJiaZi,
                           int& nYearJiaZi) {
    int nStart = nJie;
    if ((nStart % 2) == 0) {
        nJie = cb::getRemainder(24, ++nStart);
    } else {
        int nm = nMon + 1;
        if (nm > 12) {
            ++nYear;
            nMon = 1;
        } else {
            ++nMon;
        }
        nJie = cb::getRemainder(24, ++nStart);
        if (nJie == 2) {  // 立春
            int nTem = 0;
            getNextYMGanZhi(nYearJiaZi, nTem, nMonJiaZi);
            nYearJiaZi = nTem;
        } else {
            nMonJiaZi = cb::getRemainder(60, nMonJiaZi + 1);
        }
    }
}

// 给定年干支，返回下一年的年干支和起始月干支
void CCalenderV1::getNextYMGanZhi(const int nJiaZiY, int& nJiaZiNextY,
                                int& nJiaZiNextM) {
    nJiaZiNextY = cb::getRemainder(60, nJiaZiY + 1);
    int nTian = nJiaZiNextY % 10;
    int nMonStart = m_wuhu[nTian];
    nJiaZiNextM = cb::getRemainder(10, (nMonStart - 2)) * 6 + 2;
}

// 获取上一个月的年月干支，最后一个参数为当月是否是 立春
void CCalenderV1::getPreMonYMGanZhi(const int nJiaZiY, const int nJiaZiM,
                                  int& nJiaZiYOut, int& nJiaZiMOut,
                                  bool isLiChun) {
    if (isLiChun) {
        nJiaZiYOut = cb::getRemainder(60, nJiaZiY - 1);
        int nTianGan = nJiaZiYOut % 10;
        int nMonStart = m_wuhu[nTianGan];
        nJiaZiMOut = cb::getRemainder(10, (nMonStart - 2)) * 6 + 2;
    } else {
        nJiaZiYOut = nJiaZiY;
        nJiaZiMOut = cb::getRemainder(60, nJiaZiM - 1);
    }
}

// 获取某年第 x 个节气为几号(从 0 小寒算起)
int CCalenderV1::getLunarSterm(int nYear, int nth, CDateTime& datetime) {
    int nYeard = nYear - 1900;
    int nUp = nYeard * 24 + nth;

    long long nRec = 0;
    for (int i = 0; i < nUp; ++i) {
        nRec = nRec + m_pJieSum[i];
    }
    CDateTime d;
    d.m_date.m_nYear = 1900;
    d.m_date.m_nMon = 1;
    d.m_date.m_nDay = 6;
    d.m_time.m_nHour = 2;
    d.m_time.m_nMin = 3;
    d.m_time.m_nSec = 57;
    getDateTimeBySecond(d, datetime, nRec);
    return datetime.m_date.m_nDay;
}

// 返回公历日期的后一天日期
void CCalenderV1::getNextDay(CDateTime& datetime) {
    CDateTime datetimeTem;
    CCalenderV1::getNextDay(datetime, datetimeTem);
    copyDateTime(datetimeTem, datetime);
}

void CCalenderV1::getNextDay(CDate& date) {
    CDateTime datetimeTemBase(date);
    CDateTime datetimeOut;
    CCalenderV1::getNextDay(datetimeTemBase, datetimeOut);
    date = datetimeOut.m_date;
}

// 返回公历日期的前一天日期
void CCalenderV1::getPreDay(CDateTime& datetime) {
    CDateTime datetimeTem;
    getPreDay(datetime, datetimeTem);
    copyDateTime(datetimeTem, datetime);
}

void CCalenderV1::getPreDay(CDate& date) {
    CDateTime datetimeTemBase(date);
    CDateTime datetimeOut;
    getPreDay(datetimeTemBase, datetimeOut);
    date = datetimeOut.m_date;
}
// 返回公历日期的前一天日期
void CCalenderV1::getPreDay(const CDateTime& datetime, CDateTime& outtime) {
    int y = datetime.m_date.m_nYear;
    int m = datetime.m_date.m_nMon;
    int d = datetime.m_date.m_nDay;

    outtime.m_time.m_nHour = datetime.m_time.m_nHour;
    outtime.m_time.m_nMin = datetime.m_time.m_nMin;
    outtime.m_time.m_nSec = datetime.m_time.m_nSec;

    if (d > 1) {
        outtime.m_date.m_nYear = datetime.m_date.m_nYear;
        outtime.m_date.m_nMon = datetime.m_date.m_nMon;
        outtime.m_date.m_nDay = datetime.m_date.m_nDay - 1;
        return;
    }
    if (m > 1) {
        outtime.m_date.m_nYear = y;
        if ((m - 1) == 2) {
            outtime.m_date.m_nMon = 2;
            if (CCalenderBase::isLeapYear(y)) {
                outtime.m_date.m_nDay = 29;
            } else {
                outtime.m_date.m_nDay = 28;
            }
            return;
        }
        outtime.m_date.m_nMon = m - 1;
        switch (outtime.m_date.m_nMon) {
            case 3:
            case 5:
            case 7:
            case 8:
            case 10: {
                outtime.m_date.m_nDay = 31;
                break;
            }
            default: {
                outtime.m_date.m_nDay = 30;
                break;
            }
        }
        return;
    }
    outtime.m_date.m_nYear = y - 1;
    outtime.m_date.m_nMon = 12;
    outtime.m_date.m_nDay = 31;
}

void CCalenderV1::getDateTimeBySecond(const CDateTime& basetime,
                                    CDateTime& outtime, long long nSecond) {
    int year = basetime.m_date.m_nYear;
    int mon = basetime.m_date.m_nMon;
    int day = basetime.m_date.m_nDay;
    int hour = basetime.m_time.m_nHour;
    int min = basetime.m_time.m_nMin;
    int sec = basetime.m_time.m_nSec;

    char      c;
    long long n = 0;
    long long num = nSecond / (24 * 3600);
    int       nRemain = static_cast<int>(nSecond % (24 * 3600));
    if (nRemain < 0) {
        nRemain = -nRemain;
    }
    int bsec = hour * 3600 + min * 60 + sec;
    if (num < 0) {
        c = '-';
        n = -num;
    } else {
        c = '+';
        n = num;
    }
    for (int i = 0; i < n; ++i) {
        if (c == '-') {
            --day;
            if (day == 0) {
                --mon;
                if (mon == 0) {
                    mon = 12;
                    --year;
                }
                switch (mon) {
                    case 4:
                    case 6:
                    case 9:
                    case 11: {
                        day = 30;
                        break;
                    }
                    case 2: {
                        if (CCalenderBase::isLeapYear(year)) {
                            day = 29;
                        } else {
                            day = 28;
                        }
                        break;
                    }
                    default: {
                        day = 31;
                        break;
                    }
                }
            }
        } else {
            ++day;
            switch (mon) {
                case 4:
                case 6:
                case 9:
                case 11: {
                    if (day == 31) {
                        day = 1;
                        ++mon;
                    }
                    break;
                }
                case 2: {
                    if (CCalenderBase::isLeapYear(year)) {
                        if (day == 30) {
                            day = 1;
                            ++mon;
                        }
                    } else {
                        if (day == 29) {
                            day = 1;
                            ++mon;
                        }
                    }
                    break;
                }
                default: {
                    if (day == 32) {
                        day = 1;
                        ++mon;
                    }
                    break;
                }
            }
            if (mon == 13) {
                mon = 1;
                ++year;
            }
        }
    }
    if (c == '-') {
        int nDiv = bsec - nRemain;
        if (nDiv >= 0) {
            outtime.m_time.m_nHour = nDiv / 3600;
            outtime.m_time.m_nMin = (nDiv % 3600) / 60;
            outtime.m_time.m_nSec = nDiv % 60;
        } else {
            getPreDay(basetime, outtime);
            int nt = 24 * 3600 + nDiv;
            outtime.m_time.m_nHour = nt / 3600;
            outtime.m_time.m_nMin = (nt % 3600) / 60;
            outtime.m_time.m_nSec = nt % 60;
            return;
        }
    } else {
        int nDiv = bsec + nRemain;
        if (nDiv > (24 * 3600)) {
            CDateTime temDate(year, mon, day, 12, 0, 0);
            CCalenderV1::getNextDay(temDate, outtime);
            nDiv -= 24 * 3600;
            outtime.m_time.m_nHour = nDiv / 3600;
            outtime.m_time.m_nMin = (nDiv % 3600) / 60;
            outtime.m_time.m_nSec = nDiv % 60;
            return;
        }
        outtime.m_time.m_nHour = nDiv / 3600;
        outtime.m_time.m_nMin = (nDiv % 3600) / 60;
        outtime.m_time.m_nSec = nDiv % 60;
    }
    outtime.m_date.m_nYear = year;
    outtime.m_date.m_nMon = mon;
    outtime.m_date.m_nDay = day;
}


// 返回距离 00:00:00 的秒数
int CCalenderV1::getSecondsFromBase(const CTime& time) {
    return time.m_nHour * 3600 + time.m_nMin * 60 + time.m_nSec;
}

// 返回两个日期之间的天数差
int CCalenderV1::getDiffByTwoDate(const CDate& dateA, const CDate& dateB) {
    int nBaseA = getDaysFromBase(dateA);
    int nBaseB = getDaysFromBase(dateB);
    return nBaseA - nBaseB;
}

// 返回两个时间之间的秒数差
int CCalenderV1::getDiffByTwoTime(const CTime& timeA, const CTime& timeB) {
    int nBaseA = getSecondsFromBase(timeA);
    int nBaseB = getSecondsFromBase(timeB);
    return nBaseA - nBaseB;
}

// 返回两个日期时间的秒数差
long long CCalenderV1::getSecondByTwoDateTime(const CDateTime& datetimeA,
                                                const CDateTime& datetimeB) {
    long long nBaseA = getDiffByTwoTime(datetimeA.m_time, datetimeB.m_time);
    long long nBaseB = getDiffByTwoDate(datetimeA.m_date, datetimeB.m_date);
    return (nBaseA + nBaseB * 86400);
}

int CCalenderV1::getDaysFromBase(const CDate& date) {
    int y = date.m_nYear;
    int m = date.m_nMon;
    int d = date.m_nDay;

    int ny = 0, nm = 0, nd = 0, nsum = 0;

    if (y >= 1900) {
        for (int i = 1900; i < y; ++i) {
            if (CCalenderBase::isLeapYear(i)) {
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
                    if (CCalenderBase::isLeapYear(y)) {
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
            if (CCalenderBase::isLeapYear(i)) {
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
                    if (CCalenderBase::isLeapYear(y)) {
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
void CCalenderV1::getNextDay(const CDateTime& datetime, CDateTime& outtime) {
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
            if (CCalenderBase::isLeapYear(y)) {
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

}  // namespace cppbox
