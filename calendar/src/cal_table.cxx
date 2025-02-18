#include "cal_table.h"
#include "table_data.h"

namespace csp {

CalendarTable::CalendarTable()
{
    init_data();
}

CalendarTable::~CalendarTable()
{
}

bool CalendarTable::generate(const DateTime& dt)
{
    if (!is_support(dt)) {
        return false;
    }
    // 这里要特殊处理一下，当23点后，农历要计为次日
    DateTime handled = dt;
    if (dt.hour == 23) {
        next_day(handled, 1);
        handled.hour = 0;
    }
    solar_ = dt;

    DateTime dtt{};
    dtt.year = 1900;
    dtt.mon = 1;
    dtt.day = 31;

    int offset = days_difference(solar_, dtt);
    int i = 0, leap_num = 0, c = 0;
    for (i = 1900; i < 2100 && offset > 0; ++i) {
        c = get_lunar_days(i);
        offset -= c;
    }
    if (offset < 0) {
        offset += c;
        --i;
    }

    lunar_.year = i;
    leap_num = get_leap_mon(i);
    leap_ = false;

    for (i = 1; i < 13 && offset > 0; ++i) {
        // 闰月
        if (leap_num > 0 && i == (leap_num + 1) && (!leap_)) {
            --i;
            leap_ = true;
            c = get_leap_days(lunar_.year);
        } else {
            c = get_lunar_day(lunar_.year, i);
        }
        // 解除闰月
        if (leap_ && i == (leap_num + 1)) {
            leap_ = false;
        }
        offset -= c;
    }

    if (offset == 0 && leap_num > 0 && i == leap_num + 1) {
        if (leap_) {
            leap_ = false;
        } else {
            leap_ = true;
            --i;
        }
    }

    if (offset < 0) {
        offset += c;
        --i;
    }

    lunar_.mon = i;
    lunar_.day = offset + 1;
    big_mon_ = leap_ ? get_leap_days(lunar_.year) != 0 : get_lunar_day(lunar_.year, lunar_.mon) > 29;

    CSPT tyear = solar_.year;
    CSPT tmon = solar_.mon;

    if (tmon == 1) {
        tyear -= 1;
        tmon = 12;
    }

    auto jq_gen = [&](CSPT year, CSPT mon, DateTime& dt, JieQi& q, bool f) {
        if (f) {
            get_lunar_sterm(year, (mon - 1) * 2, dt);
            q.index = (dt.mon - 1) * 2;
        } else {
            get_lunar_sterm(year, (mon - 1) * 2 + 1, dt);
            q.index = (dt.mon - 1) * 2 + 1;
        }
    };

    jq_gen(tyear, tmon, jie_[0].dt, jie_[0], true);
    jq_gen(tyear, tmon, jie_[1].dt, jie_[1], false);

    tyear = solar_.year;
    tmon = solar_.mon;

    jq_gen(tyear, tmon, jie_[2].dt, jie_[2], true);
    jq_gen(tyear, tmon, jie_[3].dt, jie_[3], false);

    if (tmon == 12) {
        tyear += 1;
        tmon = 1;
    }

    jq_gen(tyear, tmon, jie_[4].dt, jie_[4], true);
    jq_gen(tyear, tmon, jie_[5].dt, jie_[5], false);

    // 计算日柱
    int day_cnt = get_days_from_base(solar_);
    if (solar_.hour == 23) {
        ++day_cnt;
    }
    ganzhi.dg = day_cnt % 10;
    ganzhi.dz = (day_cnt + 10) % 12;

    cal_ym_ganzhi();
    cal_hour_ganzhi();

    return true;
}

bool CalendarTable::is_support(const DateTime& dt)
{
    if (!Calendar::check_format_only(dt)) {
        return false;
    }
    if (dt.year < 1901 || dt.year > 2099) {
        return false;
    }
    return true;
}

void CalendarTable::pre_day(DateTime& dt, unsigned int cnt)
{
    auto pre = [this, &dt]() {
        int y = dt.year;
        int m = dt.mon;
        int d = dt.day;

        if (d > 1) {
            --dt.day;
            return;
        }
        if (m > 1) {
            if ((m - 1) == 2) {
                dt.mon = 2;
                if (Calendar::is_leap(y)) {
                    dt.day = 29;
                } else {
                    dt.day = 28;
                }
                return;
            }
            dt.mon = m - 1;
            switch (dt.mon) {
            case 3:
            case 5:
            case 7:
            case 8:
            case 10: {
                dt.day = 31;
                break;
            }
            default: {
                dt.day = 30;
                break;
            }
            }
            return;
        }
        dt.year = y - 1;
        dt.mon = 12;
        dt.day = 31;
    };
    for (unsigned int i = 0; i < cnt; ++i) {
        pre();
    }
}

void CalendarTable::next_day(DateTime& dt, unsigned int cnt)
{
    auto next = [this, &dt]() {
        int y = dt.year;
        int m = dt.mon;
        int d = dt.day;

        int day = 0;
        switch (m) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12: {
            day = 31;
            break;
        }
        case 2: {
            if (Calendar::is_leap(y)) {
                day = 29;
            } else {
                day = 28;
            }
            break;
        }
        default: {
            day = 30;
            break;
        }
        }

        if (m <= 11) {
            if (day == d) {
                dt.mon = m + 1;
                dt.day = 1;
            } else {
                dt.mon = m;
                dt.day = d + 1;
            }
        } else if (m == 12) {
            if (d == day) {
                dt.year = y + 1;
                dt.mon = 1;
                dt.day = 1;
            } else {
                dt.mon = 12;
                dt.day = d + 1;
            }
        }
    };
    for (unsigned int i = 0; i < cnt; ++i) {
        next();
    }
}

CSPT CalendarTable::days_difference(const DateTime& l, const DateTime& r)
{
    return CSPT();
}

DateTime CalendarTable::cal_new_date(const DateTime& base, CSPLT secs)
{
    auto result = base;
    constexpr auto one_day_secs = 24 * 3600;

    int year = base.year;
    int mon = base.mon;
    int day = base.day;
    int hour = base.hour;
    int min = base.min;
    int sec = base.sec;

    char c{};
    CSPLT n = 0;
    CSPLT num = secs / (24 * 3600);
    int nr = static_cast<int>(secs % (24 * 3600));

    // 这里当时我为啥要取反来着？
    // if (nr < 0) {
    //     nr = -nr;
    // }

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
                    if (Calendar::is_leap(year)) {
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
                if (Calendar::is_leap(year)) {
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
        int nd = bsec - nr;
        if (nd >= 0) {
            result.hour = nd / 3600;
            result.min = (nd % 3600) / 60;
            result.sec = nd % 60;
        } else {
            pre_day(result, 1);
            int nt = one_day_secs + nd;
            result.hour = nt / 3600;
            result.min = (nt % 3600) / 60;
            result.sec = nt % 60;
            return result;
        }
    } else {
        int nd = bsec + nr;
        if (nd > one_day_secs) {
            result.year = year;
            result.mon = mon;
            result.day = day;
            result.hour = 12;
            result.min = 0;
            result.sec = 0;
            next_day(result, 1);
            nd -= one_day_secs;
            result.hour = nd / 3600;
            result.min = (nd % 3600) / 60;
            result.sec = nd % 60;
            return result;
        }
        result.hour = nd / 3600;
        result.min = (nd % 3600) / 60;
        result.sec = nd % 60;
    }
    result.year = year;
    result.mon = mon;
    result.day = day;
    return result;
}

CSPLT CalendarTable::cal_zero_secs(const DateTime& dt)
{
    return dt.hour * 3600 + dt.min * 60 + dt.sec;
}

CSPT CalendarTable::calsecs_by_time(const DateTime& l, const DateTime& r)
{
    int a = cal_zero_secs(l);
    int b = cal_zero_secs(r);
    return a - b;
}

CSPLT CalendarTable::calsecs_by_all(const DateTime& l, const DateTime& r)
{
    CSPLT a = calsecs_by_time(l, r);
    CSPLT b = days_difference(l, r);
    return (a + b * 86400);
}

void CalendarTable::init_data()
{
    jq_data_init(difference_jq_);
    lunar_data_init(lunar_table_);
}

CSPT CalendarTable::get_leap_mon(CSPT year)
{
    int r = lunar_table_[year - 1900] & 0xf;
    if (r == 0xf) {
        return 0;
    }
    return r;
}

CSPT CalendarTable::get_leap_days(CSPT year)
{
    if (get_leap_mon(year)) {
        int r = lunar_table_[year - 1899] & 0xf;
        if (r == 0xf) {
            return 30;
        }
        return 29;
    }
    return 0;
}

CSPT CalendarTable::get_lunar_days(CSPT year)
{
    int i = 0, s = 348, n = 0;
    for (i = 0x8000; i > 0x8; i >>= 1) {
        n = lunar_table_[year - 1900];
        s += (n & i) ? 1 : 0;
    }
    return s + get_leap_days(year);
}

CSPT CalendarTable::get_lunar_day(CSPT year, CSPT mon)
{
    int r = lunar_table_[year - 1900] & (0x10000 >> mon);
    if (r) {
        return 30;
    }
    return 29;
}

CSPT CalendarTable::get_lunar_sterm(CSPT year, CSPT cnt, DateTime& datetime)
{
    CSPT up = (year - 1900) * 24 + cnt;

    CSPLT n = 0;
    for (int i = 0; i < up; ++i) {
        n = n + difference_jq_[i];
    }

    DateTime d;
    d.year = 1900;
    d.mon = 1;
    d.day = 6;
    d.hour = 2;
    d.min = 3;
    d.sec = 57;

    datetime = cal_new_date(d, n);
    return datetime.day;
}

void CalendarTable::cal_ym_ganzhi()
{
    /*
        以公历 1900 年 01 月 6 日时为起始日期(小寒）对应农历日期为 1900 年 己亥年，丁丑月 (以立春为新年)
    */

    int year_gz = 35;   // 己亥 的索引
    int mon_gz = 13;    // 丁丑 的索引　
    int sjq = 0;        // 小寒 的索引
    int sy = 1900;
    int sm = 1;

    const auto& first_jq = jie_[2];
    int cnt = (solar_.year - 1900) * 24 + first_jq.index;
    for (int i = 0; i < cnt; ++i) {
        next_mon_info(sy, sm, sjq, mon_gz, year_gz);
    }
    auto nChange = days_difference(first_jq.dt, solar_);
    int mg = 0, mk = 0;
    if (nChange < 0) {
        mg = year_gz;
        mk = mon_gz;
    } else {
        if (first_jq.index == 2) {
            pre_mon_info(year_gz, mon_gz, mg, mk, true);
        } else {
            pre_mon_info(year_gz, mon_gz, mg, mk, false);
        }
    }
    ganzhi.yg = mg % 10;
    ganzhi.yz = mg % 12;
    ganzhi.mg = mk % 10;
    ganzhi.mz = mk % 12;
}

void CalendarTable::cal_hour_ganzhi()
{
    int ns = qishi_[ganzhi.dg];
    int ng = Calendar::mod(10, ns) * 6;
    int jh = solar_.hour + 1;
    if (jh == 24) {
        jh = 0;
    }
    ganzhi.hg = Calendar::mod(10, ng + jh / 2);
    ganzhi.hz = jh / 2;
}

CSPT CalendarTable::get_days_from_base(const DateTime& dt)
{
    int y = dt.year;
    int m = dt.mon;
    int d = dt.day;

    int ny = 0, nm = 0, nd = 0, nsum = 0;

    if (y >= 1900) {
        for (int i = 1900; i < y; ++i) {
            if (Calendar::is_leap(i)) {
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
                if (Calendar::is_leap(y)) {
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
            if (Calendar::is_leap(i)) {
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
                if (Calendar::is_leap(y)) {
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

void CalendarTable::next_mon_info(CSPT& year, CSPT& mon, CSPT& jie, CSPT& jz_mon, CSPT& jz_year)
{
    CSPT ns = jie;
    if ((ns % 2) == 0) {
        jie = Calendar::mod(24, ++ns);
    } else {
        if ((mon + 1) > 12) {
            ++year;
            mon = 1;
        } else {
            ++mon;
        }
        jie = Calendar::mod(24, ++ns);
        if (jie == 2) {   // 立春
            CSPT n = 0;
            next_year_info(jz_year, n, jz_mon);
            jz_year = n;
        } else {
            jz_mon = Calendar::mod(60, jz_mon + 1);
        }
    }
}

void CalendarTable::pre_mon_info(CSPT jz_year, CSPT jz_mon, CSPT& jz_year_out, CSPT& jz_mon_out, bool lichun)
{
    if (lichun) {
        jz_year_out = Calendar::mod(60, jz_year - 1);
        int tian = jz_year_out % 10;
        int ms = wuhu_[tian];
        jz_mon_out = Calendar::mod(10, (ms - 2)) * 6 + 2;
    } else {
        jz_year_out = jz_year;
        jz_mon_out = Calendar::mod(60, jz_mon - 1);
    }
}

void CalendarTable::next_year_info(CSPT jz_year, CSPT& jz_year_out, CSPT& jz_mon_out)
{
    jz_year_out = Calendar::mod(60, jz_year + 1);
    int tian = jz_year_out % 10;
    int ms = wuhu_[tian];
    jz_mon_out = Calendar::mod(10, (ms - 2)) * 6 + 2;
}

}   // namespace csp