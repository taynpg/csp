#include "calendar_v1.h"

#include "calendar_v1_data.h"

namespace cppbox {

CCalenderV1::CCalenderV1() { initSelf(); }

CCalenderV1::~CCalenderV1()
{
    FreeIntList(m_pJieSum);
    FreeIntList(m_pLunar);
}

void CCalenderV1::initSelf()
{
    if (!AllocJieQi(&m_pJieSum)) {
        return;
    }
    if (!AllocLunar(&m_pLunar)) {
        return;
    }
    leap_ = false;
    big_mon_ = false;

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

bool CCalenderV1::set_datetime(const CDateTime& datetime)
{
    // 检查日期格式是否合法
    if (!check_format(datetime)) {
        return false;
    }

    mcopy(datetime, dt_);
    CDate sdate;
    sdate.year_ = 1900;
    sdate.mon_ = 1;
    sdate.day_ = 31;
    int offset = get_diff_day(dt_.date_, sdate);
    int i = 0, nleap = 0, ntem = 0;
    for (i = 1900; i < 2100 && offset > 0; ++i) {
        ntem = getLunarDays(i);
        offset -= ntem;
    }
    if (offset < 0) {
        offset += ntem;
        --i;
    }

    lunar_.date_.year_ = i;
    nleap = getLeapMon(i);
    leap_ = false;

    for (i = 1; i < 13 && offset > 0; ++i) {
        // 闰月
        if (nleap > 0 && i == (nleap + 1) && (!leap_)) {
            --i;
            leap_ = true;
            ntem = getLeapDays(lunar_.date_.year_);
        } else {
            ntem = getLunarDay(lunar_.date_.year_, i);
        }
        // 解除闰月
        if (leap_ && i == (nleap + 1)) {
            leap_ = false;
        }
        offset -= ntem;
    }

    if (offset == 0 && nleap > 0 && i == nleap + 1) {
        if (leap_) {
            leap_ = false;
        } else {
            leap_ = true;
            --i;
        }
    }

    if (offset < 0) {
        offset += ntem;
        --i;
    }

    lunar_.date_.mon_ = i;
    lunar_.date_.day_ = offset + 1;
    big_mon_ = leap_ ? getLeapDays(lunar_.date_.year_) != 0 : getLunarDay(lunar_.date_.year_, lunar_.date_.mon_) > 29;

    // 计算节气相关
    getLunarSterm(dt_.date_.year_, (dt_.date_.mon_ - 1) * 2, first_jie_.dt_);
    first_jie_.index_ = (first_jie_.dt_.date_.mon_ - 1) * 2;

    getLunarSterm(dt_.date_.year_, (dt_.date_.mon_ - 1) * 2 + 1, second_jie_.dt_);
    second_jie_.index_ = (second_jie_.dt_.date_.mon_ - 1) * 2 + 1;

    // 计算日柱
    int nDayCnt = getDaysFromBase(dt_.date_);
    if (dt_.time_.h_ == 23) {
        ++nDayCnt;
    }
    sz_.dg_ = nDayCnt % 10;
    sz_.dz_ = (nDayCnt + 10) % 12;

    getYMGanZhi();
    getHourGanZhi();

    return true;
}

bool CCalenderV1::check_format(const CDateTime& datetime)
{
    if (datetime.date_.year_ < 1901 || datetime.date_.year_ > 2099) {
        return false;
    }
    if (datetime.date_.mon_ < 1 || datetime.date_.mon_ > 12) {
        return false;
    }

    int y = datetime.date_.year_;
    int m = datetime.date_.mon_;
    int d = datetime.date_.day_;

    if (d < 1) {
        return false;
    }

    if (CCalenderBase::is_leap(y)) {
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

// 农历指定年所闰的月数，没有返回0
int CCalenderV1::getLeapMon(int nYear)
{
    int nRet = m_pLunar[nYear - 1900] & 0xf;
    if (nRet == 0xf) {
        return 0;
    }
    return nRet;
}

// 返回农历 xx 年闰月的天数
int CCalenderV1::getLeapDays(int nYear)
{
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
int CCalenderV1::getLunarDays(int nYear)
{
    int i = 0, nsum = 348, ntem = 0;
    for (i = 0x8000; i > 0x8; i >>= 1) {
        ntem = m_pLunar[nYear - 1900];
        nsum += (ntem & i) ? 1 : 0;
    }
    return nsum + getLeapDays(nYear);
}

// 返回农历 xx 年 xx 月的天数
int CCalenderV1::getLunarDay(int nYear, int nMon)
{
    int nRe = m_pLunar[nYear - 1900] & (0x10000 >> nMon);
    if (nRe) {
        return 30;
    }
    return 29;
}

// 计算时辰的干支(在计算完年月干支后进行)
void CCalenderV1::getHourGanZhi()
{
    int nStart = m_qishi[sz_.dg_];
    int nStartGan = cb::remain(10, nStart) * 6;
    int nhour = dt_.time_.h_ + 1;
    if (nhour == 24) {
        nhour = 0;
    }
    sz_.hg_ = cb::remain(10, nStartGan + nhour / 2);
    sz_.hz_ = nhour / 2;
}

// 计算农历的年干支，月干支 (立春为新年)
void CCalenderV1::getYMGanZhi()
{
    // 以公历 1900 年 01 月 6 日时为起始日期(小寒）对应农历日期为 1900 年
    // 己亥年，丁丑月 (以立春为新年)
    int nStartYGanZhi = 35;   // 己亥 的索引
    int nStartMGanZhi = 13;   // 丁丑 的索引　
    int nJieQiIndex = 0;      // 小寒 的索引
    int nStartYear = 1900;
    int nStartMon = 1;

    int nJieQiCnt = (dt_.date_.year_ - 1900) * 24 + first_jie_.index_;
    for (int i = 0; i < nJieQiCnt; ++i) {
        getNextJie(nStartYear, nStartMon, nJieQiIndex, nStartMGanZhi, nStartYGanZhi);
    }
    long long nChange = get_sec_by_date(first_jie_.dt_, dt_);
    int       nTemA = 0, nTemB = 0;
    if (nChange < 0) {
        nTemA = nStartYGanZhi;
        nTemB = nStartMGanZhi;
    } else {
        if (first_jie_.index_ == 2) {
            getPreMonYMGanZhi(nStartYGanZhi, nStartMGanZhi, nTemA, nTemB, true);
        } else {
            getPreMonYMGanZhi(nStartYGanZhi, nStartMGanZhi, nTemA, nTemB, false);
        }
    }
    sz_.yg_ = nTemA % 10;
    sz_.yz_ = nTemA % 12;
    sz_.mg_ = nTemB % 10;
    sz_.mz_ = nTemB % 12;
}
// 返回下一个月的年干支月干支信息
void CCalenderV1::getNextJie(int& nYear, int& nMon, int& nJie, int& nMonJiaZi, int& nYearJiaZi)
{
    int nStart = nJie;
    if ((nStart % 2) == 0) {
        nJie = cb::remain(24, ++nStart);
    } else {
        int nm = nMon + 1;
        if (nm > 12) {
            ++nYear;
            nMon = 1;
        } else {
            ++nMon;
        }
        nJie = cb::remain(24, ++nStart);
        if (nJie == 2) {   // 立春
            int nTem = 0;
            getNextYMGanZhi(nYearJiaZi, nTem, nMonJiaZi);
            nYearJiaZi = nTem;
        } else {
            nMonJiaZi = cb::remain(60, nMonJiaZi + 1);
        }
    }
}

// 给定年干支，返回下一年的年干支和起始月干支
void CCalenderV1::getNextYMGanZhi(const int nJiaZiY, int& nJiaZiNextY, int& nJiaZiNextM)
{
    nJiaZiNextY = cb::remain(60, nJiaZiY + 1);
    int nTian = nJiaZiNextY % 10;
    int nMonStart = m_wuhu[nTian];
    nJiaZiNextM = cb::remain(10, (nMonStart - 2)) * 6 + 2;
}

// 获取上一个月的年月干支，最后一个参数为当月是否是 立春
void CCalenderV1::getPreMonYMGanZhi(const int nJiaZiY, const int nJiaZiM, int& nJiaZiYOut, int& nJiaZiMOut, bool isLiChun)
{
    if (isLiChun) {
        nJiaZiYOut = cb::remain(60, nJiaZiY - 1);
        int nTianGan = nJiaZiYOut % 10;
        int nMonStart = m_wuhu[nTianGan];
        nJiaZiMOut = cb::remain(10, (nMonStart - 2)) * 6 + 2;
    } else {
        nJiaZiYOut = nJiaZiY;
        nJiaZiMOut = cb::remain(60, nJiaZiM - 1);
    }
}

// 获取某年第 x 个节气为几号(从 0 小寒算起)
int CCalenderV1::getLunarSterm(int nYear, int nth, CDateTime& datetime)
{
    int nYeard = nYear - 1900;
    int nUp = nYeard * 24 + nth;

    long long nRec = 0;
    for (int i = 0; i < nUp; ++i) {
        nRec = nRec + m_pJieSum[i];
    }
    CDateTime d;
    d.date_.year_ = 1900;
    d.date_.mon_ = 1;
    d.date_.day_ = 6;
    d.time_.h_ = 2;
    d.time_.m_ = 3;
    d.time_.s_ = 57;
    get_diff_sec(d, datetime, nRec);
    return datetime.date_.day_;
}

// 返回公历日期的后一天日期
void CCalenderV1::next(CDateTime& datetime)
{
    CDateTime datetimeTem;
    CCalenderV1::getNextDay(datetime, datetimeTem);
    mcopy(datetimeTem, datetime);
}

void CCalenderV1::next(CDate& date)
{
    CDateTime datetimeTemBase(date);
    CDateTime datetimeOut;
    CCalenderV1::getNextDay(datetimeTemBase, datetimeOut);
    date = datetimeOut.date_;
}

// 返回公历日期的前一天日期
void CCalenderV1::pre(CDateTime& datetime)
{
    CDateTime datetimeTem;
    getPreDay(datetime, datetimeTem);
    mcopy(datetimeTem, datetime);
}

void CCalenderV1::pre(CDate& date)
{
    CDateTime datetimeTemBase(date);
    CDateTime datetimeOut;
    getPreDay(datetimeTemBase, datetimeOut);
    date = datetimeOut.date_;
}
// 返回公历日期的前一天日期
void CCalenderV1::getPreDay(const CDateTime& datetime, CDateTime& outtime)
{
    int y = datetime.date_.year_;
    int m = datetime.date_.mon_;
    int d = datetime.date_.day_;

    outtime.time_.h_ = datetime.time_.h_;
    outtime.time_.m_ = datetime.time_.m_;
    outtime.time_.s_ = datetime.time_.s_;

    if (d > 1) {
        outtime.date_.year_ = datetime.date_.year_;
        outtime.date_.mon_ = datetime.date_.mon_;
        outtime.date_.day_ = datetime.date_.day_ - 1;
        return;
    }
    if (m > 1) {
        outtime.date_.year_ = y;
        if ((m - 1) == 2) {
            outtime.date_.mon_ = 2;
            if (CCalenderBase::is_leap(y)) {
                outtime.date_.day_ = 29;
            } else {
                outtime.date_.day_ = 28;
            }
            return;
        }
        outtime.date_.mon_ = m - 1;
        switch (outtime.date_.mon_) {
        case 3:
        case 5:
        case 7:
        case 8:
        case 10: {
            outtime.date_.day_ = 31;
            break;
        }
        default: {
            outtime.date_.day_ = 30;
            break;
        }
        }
        return;
    }
    outtime.date_.year_ = y - 1;
    outtime.date_.mon_ = 12;
    outtime.date_.day_ = 31;
}

void CCalenderV1::get_diff_sec(const CDateTime& basetime, CDateTime& outtime, long long nSecond)
{
    int year = basetime.date_.year_;
    int mon = basetime.date_.mon_;
    int day = basetime.date_.day_;
    int hour = basetime.time_.h_;
    int min = basetime.time_.m_;
    int sec = basetime.time_.s_;

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
                    if (CCalenderBase::is_leap(year)) {
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
                if (CCalenderBase::is_leap(year)) {
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
            outtime.time_.h_ = nDiv / 3600;
            outtime.time_.m_ = (nDiv % 3600) / 60;
            outtime.time_.s_ = nDiv % 60;
        } else {
            getPreDay(basetime, outtime);
            int nt = 24 * 3600 + nDiv;
            outtime.time_.h_ = nt / 3600;
            outtime.time_.m_ = (nt % 3600) / 60;
            outtime.time_.s_ = nt % 60;
            return;
        }
    } else {
        int nDiv = bsec + nRemain;
        if (nDiv > (24 * 3600)) {
            CDateTime temDate(year, mon, day, 12, 0, 0);
            CCalenderV1::getNextDay(temDate, outtime);
            nDiv -= 24 * 3600;
            outtime.time_.h_ = nDiv / 3600;
            outtime.time_.m_ = (nDiv % 3600) / 60;
            outtime.time_.s_ = nDiv % 60;
            return;
        }
        outtime.time_.h_ = nDiv / 3600;
        outtime.time_.m_ = (nDiv % 3600) / 60;
        outtime.time_.s_ = nDiv % 60;
    }
    outtime.date_.year_ = year;
    outtime.date_.mon_ = mon;
    outtime.date_.day_ = day;
}

// 返回距离 00:00:00 的秒数
int CCalenderV1::get_sec_by_base(const CTime& time) { return time.h_ * 3600 + time.m_ * 60 + time.s_; }

// 返回两个日期之间的天数差
int CCalenderV1::get_diff_day(const CDate& dateA, const CDate& dateB)
{
    int nBaseA = getDaysFromBase(dateA);
    int nBaseB = getDaysFromBase(dateB);
    return nBaseA - nBaseB;
}

// 返回两个时间之间的秒数差
int CCalenderV1::get_diff_by_time(const CTime& timeA, const CTime& timeB)
{
    int nBaseA = get_sec_by_base(timeA);
    int nBaseB = get_sec_by_base(timeB);
    return nBaseA - nBaseB;
}

// 返回两个日期时间的秒数差
long long CCalenderV1::get_sec_by_date(const CDateTime& datetimeA, const CDateTime& datetimeB)
{
    long long nBaseA = get_diff_by_time(datetimeA.time_, datetimeB.time_);
    long long nBaseB = get_diff_day(datetimeA.date_, datetimeB.date_);
    return (nBaseA + nBaseB * 86400);
}

int CCalenderV1::getDaysFromBase(const CDate& date)
{
    int y = date.year_;
    int m = date.mon_;
    int d = date.day_;

    int ny = 0, nm = 0, nd = 0, nsum = 0;

    if (y >= 1900) {
        for (int i = 1900; i < y; ++i) {
            if (CCalenderBase::is_leap(i)) {
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
                if (CCalenderBase::is_leap(y)) {
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
            if (CCalenderBase::is_leap(i)) {
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
                if (CCalenderBase::is_leap(y)) {
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
void CCalenderV1::getNextDay(const CDateTime& datetime, CDateTime& outtime)
{
    int y = datetime.date_.year_;
    int m = datetime.date_.mon_;
    int d = datetime.date_.day_;

    outtime.time_.h_ = datetime.time_.h_;
    outtime.time_.m_ = datetime.time_.m_;
    outtime.time_.s_ = datetime.time_.s_;

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
        if (CCalenderBase::is_leap(y)) {
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
        outtime.date_.year_ = y;
        if (nDay == d) {
            outtime.date_.mon_ = m + 1;
            outtime.date_.day_ = 1;
        } else {
            outtime.date_.mon_ = m;
            outtime.date_.day_ = d + 1;
        }
    } else if (m == 12) {
        if (d == nDay) {
            outtime.date_.year_ = y + 1;
            outtime.date_.mon_ = 1;
            outtime.date_.day_ = 1;
        } else {
            outtime.date_.year_ = y;
            outtime.date_.mon_ = 12;
            outtime.date_.day_ = d + 1;
        }
    }
}

}   // namespace cppbox
