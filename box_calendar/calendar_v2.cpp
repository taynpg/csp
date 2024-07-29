#include "calendar_v2.h"

namespace cppbox {

CCalenderV2::CCalenderV2() {}
CCalenderV2::~CCalenderV2() { delete day_; }

bool CCalenderV2::set_datetime(const CDateTime& datetime)
{
    dt_ = datetime;
    day_ = sxtwl::fromSolar(dt_.date_.year_, dt_.date_.mon_, dt_.date_.day_);

    lunar_.time_ = dt_.time_;
    sx2lunar(day_, lunar_.date_);

    sz_.yg_ = day_->getYearGZ().tg;
    sz_.mg_ = day_->getMonthGZ().tg;
    sz_.dg_ = day_->getDayGZ().tg;

    sz_.yz_ = day_->getYearGZ().dz;
    sz_.mz_ = day_->getMonthGZ().dz;
    sz_.dz_ = day_->getDayGZ().dz;

    GZ gz = sxtwl::getShiGz(sz_.dg_, dt_.time_.h_);
    sz_.hg_ = gz.tg;
    sz_.hz_ = gz.dz;

    leap_ = day_->isLunarLeap();
    std::vector<sxtwl::JieQiInfo> jq_vec{};
    if (dt_.date_.mon_ == 1) {
        jq_vec = sxtwl::getJieQiByYear(dt_.date_.year_ - 1);
    } else {
        jq_vec = sxtwl::getJieQiByYear(dt_.date_.year_);
    }

    std::vector<sxtwl::JieQiInfo>::const_iterator it;
    for (it = jq_vec.begin(); it != jq_vec.end(); ++it) {
        Time time = sxtwl::JD2DD(it->jd);
        if (time.getMonth() != dt_.date_.mon_) {
            continue;
        }
        // sxtwl 的 jqindex 是从冬至开始的，而 box_zhdata 是从
        // 小寒开始的，所以这里要做一下处理
        if (time.getDay() < 15) {
            sx2dt(time, first_jie_.dt_);
            first_jie_.index_ = CCalenderBase::remain(24, it->jqIndex - 1);
        } else {
            sx2dt(time, second_jie_.dt_);
            second_jie_.index_ = CCalenderBase::remain(24, it->jqIndex - 1);
            break;
        }
    }
    check_mon_gz();
    return true;
}

// 根据节气时间修正月干支
void CCalenderV2::check_mon_gz()
{
    auto ck = [&](const CDateTime& datetime) {
        if (dt_.date_ != datetime.date_) {
            return ;
        }
        int df = get_diff_by_time(datetime.time_, datetime.time_);
        if (df < 0) {
            sz_.mg_ = CCalenderBase::remain(10, sz_.mg_ - 1);
            sz_.mz_ = CCalenderBase::remain(12, sz_.mz_ - 1);
        }
    };
    ck(first_jie_.dt_);
    ck(second_jie_.dt_);
}

// 获取前一天
void CCalenderV2::pre(CDateTime& datetime) { pre(datetime.date_); }

void CCalenderV2::pre(CDate& date)
{
    Day* d = sxtwl::fromSolar(date.year_, date.mon_, date.day_);
    Day* bd = d->before(1);
    sx2solar(bd, date);
    delete d;
    delete bd;
}

// 获取后一天
void CCalenderV2::next(CDateTime& datetime) { next(datetime.date_); }

void CCalenderV2::next(CDate& date)
{
    Day* d = sxtwl::fromSolar(date.year_, date.mon_, date.day_);
    Day* bd = d->after(1);
    sx2solar(bd, date);
    delete d;
    delete bd;
}

// 检查日期格式是否正确
bool CCalenderV2::check_format(const CDateTime& datetime)
{
    if (datetime.date_.year_ < -198 || datetime.date_.year_ > 9999) {
        return false;
    }
    return true;
}

// 返回两个日期之间的天数差
int CCalenderV2::get_diff_day(const CDate& dateA, const CDate& dateB)
{
    Time   ta(dateA.year_, dateA.mon_, dateA.day_, 0, 0, 0);
    Time   tb(dateB.year_, dateB.mon_, dateB.day_, 0, 0, 0);
    double a = sxtwl::toJD(ta);
    double b = sxtwl::toJD(tb);
    double dd = a - b;
    return static_cast<int>(dd);
}

// 返回距离 00:00:00 的秒数
int CCalenderV2::get_sec_by_base(const CTime& time) { return time.h_ * 3600 + time.m_ * 60 + time.s_; }

// 基于基础时间和差值计算新的日期
void CCalenderV2::get_diff_sec(const CDateTime& basetime, CDateTime& outtime, long long nSecond) {}

// 返回两个时间之间的秒数差
int CCalenderV2::get_diff_by_time(const CTime& timeA, const CTime& timeB)
{
    int nBaseA = get_sec_by_base(timeA);
    int nBaseB = get_sec_by_base(timeB);
    return nBaseA - nBaseB;
}

// 返回两个日期时间的秒数差
long long CCalenderV2::get_sec_by_date(const CDateTime& datetimeA, const CDateTime& datetimeB)
{
    Time   ta(datetimeA.date_.year_, datetimeA.date_.mon_, datetimeA.date_.day_, datetimeA.time_.h_, datetimeA.time_.m_,
              datetimeA.time_.s_);
    Time   tb(datetimeB.date_.year_, datetimeB.date_.mon_, datetimeB.date_.day_, datetimeB.time_.h_, datetimeB.time_.m_,
              datetimeB.time_.s_);
    double a = sxtwl::toJD(ta);
    double b = sxtwl::toJD(tb);
    double dd = a - b;
    return static_cast<int>(dd * 86400);
}

void CCalenderV2::sx2dt(Time& time, CDateTime& datetime)
{
    datetime.date_.year_ = time.getYear();
    datetime.date_.mon_ = time.getMonth();
    datetime.date_.day_ = time.getDay();
    datetime.time_.h_ = static_cast<int>(time.getHour());
    datetime.time_.m_ = static_cast<int>(time.getMin());
    datetime.time_.s_ = rd(time.getSec());
}

void CCalenderV2::sx2lunar(Day* day, CDate& date)
{
    date.year_ = day->getLunarYear();
    date.mon_ = static_cast<int>(day->getLunarMonth());
    date.day_ = day->getLunarDay();
}

void CCalenderV2::sx2solar(Day* day, CDate& date)
{
    date.year_ = day->getSolarYear();
    date.mon_ = static_cast<int>(day->getSolarMonth());
    date.day_ = day->getSolarDay();
}

int CCalenderV2::rd(double number)
{
    double ret = (number > 0.0) ? (number + 0.5) : (number - 0.5);
    return static_cast<int>(ret);
}

}   // namespace cppbox
