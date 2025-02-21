#include "cal_sx.h"

namespace csp {
CalendarSxwnl::CalendarSxwnl() = default;

CalendarSxwnl::~CalendarSxwnl() = default;

bool CalendarSxwnl::generate(const DateTime& dt)
{
    if (!is_support(dt)) {
        return false;
    }

    DateTime h = dt;
    if (h.hour == 23) {
        next_day(h, 1);
        h.hour = 0;
    }
    solar_ = h;
    lunar_ = dt;
    day_.reset(sxtwl::fromSolar(solar_.year, solar_.mon, solar_.day));

    sx2lunar(day_.get(), lunar_);

    ganzhi.yg = day_->getYearGZ().tg;
    ganzhi.mg = day_->getMonthGZ().tg;
    ganzhi.dg = day_->getDayGZ().tg;
    ganzhi.yz = day_->getYearGZ().dz;
    ganzhi.mz = day_->getMonthGZ().dz;
    ganzhi.dz = day_->getDayGZ().dz;

    auto gz = sxtwl::getShiGz(ganzhi.dg, solar_.hour);
    ganzhi.hg = gz.tg;
    ganzhi.hz = gz.dz;

    if (dt.hour == 23) {
        ganzhi.dg = mod(10, ganzhi.dg + 1);
        ganzhi.dz = mod(12, ganzhi.dz + 1);
    }

    leap_ = day_->isLunarLeap();

    std::vector<sxtwl::JieQiInfo> jq_vec{};

    // 这里之所以这样处理是，接口抛出的结果，从2月开始
    if (solar_.mon == 1 || solar_.mon == 2) {
        jq_vec = sxtwl::getJieQiByYear(solar_.year - 1);
        auto aj = sxtwl::getJieQiByYear(solar_.year);
        jq_vec.reserve(jq_vec.size() + aj.size());
        std::copy(aj.begin(), aj.end(), std::back_inserter(jq_vec));
    } else {
        jq_vec = sxtwl::getJieQiByYear(solar_.year);
    }

    std::vector<std::pair<int, int>> cache;
    if (solar_.mon == 1) {
        cache.emplace_back(std::make_pair(solar_.year - 1, 12));
        cache.emplace_back(std::make_pair(solar_.year, solar_.mon));
        cache.emplace_back(std::make_pair(solar_.year, solar_.mon + 1));
    } else if (solar_.mon == 12) {
        cache.emplace_back(std::make_pair(solar_.year, solar_.mon - 1));
        cache.emplace_back(std::make_pair(solar_.year, solar_.mon));
        cache.emplace_back(std::make_pair(solar_.year + 1, 1));
    } else {
        cache.emplace_back(std::make_pair(solar_.year, solar_.mon - 1));
        cache.emplace_back(std::make_pair(solar_.year, solar_.mon));
        cache.emplace_back(std::make_pair(solar_.year, solar_.mon + 1));
    }

    std::vector<sxtwl::JieQiInfo>::const_iterator it;
    for (it = jq_vec.begin(); it != jq_vec.end(); ++it) {
        Time time = sxtwl::JD2DD(it->jd);
        // sxtwl 的 jqindex 是从冬至开始的，而 zhcn 是从
        // 小寒开始的，所以这里要做一下处理
        if (time.getYear() == cache[0].first && time.getMonth() == cache[0].second) {
            if (time.getDay() < 15) {
                sx2dt(time, jie_[0].dt);
                jie_[0].index = mod(24, it->jqIndex - 1);
            } else {
                sx2dt(time, jie_[1].dt);
                jie_[1].index = mod(24, it->jqIndex - 1);
            }
            continue;
        }
        if (time.getYear() == cache[1].first && time.getMonth() == cache[1].second) {
            if (time.getDay() < 15) {
                sx2dt(time, jie_[2].dt);
                jie_[2].index = mod(24, it->jqIndex - 1);
            } else {
                sx2dt(time, jie_[3].dt);
                jie_[3].index = mod(24, it->jqIndex - 1);
            }
            continue;
        }
        if (time.getYear() == cache[2].first && time.getMonth() == cache[2].second) {
            if (time.getDay() < 15) {
                sx2dt(time, jie_[4].dt);
                jie_[4].index = mod(24, it->jqIndex - 1);
            } else {
                sx2dt(time, jie_[5].dt);
                jie_[5].index = mod(24, it->jqIndex - 1);
            }
            continue;
        }
    }
    check_mon_gz();
    solar_ = dt;
    return true;
}

bool CalendarSxwnl::is_support(const DateTime& dt)
{
    if (dt.year < -198 || dt.year > 9999) {
        return false;
    }
    return true;
}

void CalendarSxwnl::pre_day(DateTime& dt, unsigned int cnt)
{
    std::shared_ptr<Day> d(sxtwl::fromSolar(dt.year, dt.mon, dt.day));
    std::shared_ptr<Day> pd(d->before(cnt));
    sx2solar(pd.get(), dt);
}

void CalendarSxwnl::next_day(DateTime& dt, unsigned int cnt)
{
    std::shared_ptr<Day> d(sxtwl::fromSolar(dt.year, dt.mon, dt.day));
    std::shared_ptr<Day> nd(d->after(cnt));
    sx2solar(nd.get(), dt);
}

CSPT CalendarSxwnl::days_difference(const DateTime& l, const DateTime& r)
{
    Time a(l.year, l.mon, l.day, 0, 0, 0);
    Time b(r.year, r.mon, r.day, 0, 0, 0);

    double df = sxtwl::toJD(a) - sxtwl::toJD(b);
    return static_cast<CSPT>(df);
}

DateTime CalendarSxwnl::cal_new_date(const DateTime& base, CSPLT secs)
{
    Time a(base.year, base.mon, base.day, base.hour, base.min, base.sec);
    double da = sxtwl::toJD(a);
    double db = da + static_cast<double>(secs / 86400.0);
    Time b = sxtwl::JD2DD(db);
    DateTime r{};
    sx2dt(b, r);
    return r;
}

CSPLT CalendarSxwnl::cal_zero_secs(const DateTime& dt)
{
    return dt.hour * 3600 + dt.min * 60 + dt.sec;
}

CSPT CalendarSxwnl::calsecs_by_time(const DateTime& l, const DateTime& r)
{
    CSPT a = cal_zero_secs(l);
    CSPT b = cal_zero_secs(r);
    return a - b;
}

CSPLT CalendarSxwnl::calsecs_by_all(const DateTime& l, const DateTime& r)
{
    Time a(l.year, l.mon, l.day, l.hour, l.min, l.sec);
    Time b(r.year, r.mon, r.day, r.hour, r.min, r.sec);

    double da = sxtwl::toJD(a);
    double db = sxtwl::toJD(b);
    double dr = da - db;
    return static_cast<CSPLT>(dr * 86400);
}

void CalendarSxwnl::sx2dt(const Time& t, DateTime& dt)
{
    auto ct(t);
    dt.year = ct.getYear();
    dt.mon = ct.getMonth();
    dt.day = ct.getDay();
    dt.hour = static_cast<int>(ct.getHour());
    dt.min = static_cast<int>(ct.getMin());
    dt.sec = rd(ct.getSec());
}

void CalendarSxwnl::sx2lunar(Day* day, DateTime& date)
{
    date.year = day->getLunarYear();
    date.mon = static_cast<int>(day->getLunarMonth());
    date.day = day->getLunarDay();
}

void CalendarSxwnl::sx2solar(Day* day, DateTime& date)
{
    date.year = day->getSolarYear();
    date.mon = static_cast<int>(day->getSolarMonth());
    date.day = day->getSolarDay();
}

int CalendarSxwnl::rd(double number)
{
    double ret = (number > 0.0) ? (number + 0.5) : (number - 0.5);
    return static_cast<int>(ret);
}

void CalendarSxwnl::check_mon_gz()
{
    auto check = [this](const DateTime& dt) {
        auto dif = calsecs_by_time(solar_, dt);
        if (dif < 0) {
            ganzhi.mg = mod(10, ganzhi.mg - 1);
            ganzhi.mz = mod(12, ganzhi.mz - 1);
        }
    };
    check(jie_[2].dt);
}

}   // namespace csp