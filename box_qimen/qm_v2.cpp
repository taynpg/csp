#include "qm_v2.h"

namespace cppbox {

CQimenV2::CQimenV2()
{
    pcal_ = nullptr;
    jushu_ = 0;
    cal_type_ = CalendarType::CALENDAR_V1;
}

CQimenV2::~CQimenV2() = default;

bool CQimenV2::run(const QiParam& info, CalendarType type)
{
    if (!base_run(info, type)) {
        return false;
    }
    if (info.ju_ == 0) {
        get_ju(info.datetime_);
    }

    // 这里跑一下今天的日期以供后续使用
    pcal_->set_datetime(datetime_);

    gen_dipan();
    gen_zhi();
    gen_jx();
    gen_bm();
    gen_bs();
    gen_tp();
    gen_other();

    return true;
}

void CQimenV2::get_ju(const CDateTime& datetime)
{
    CCalender* pCalendar = CCalenderFactory::creat_instance(cal_type_);
    pCalendar->set_datetime(datetime);
    // 年支数
    int zhinum = pCalendar->get_sz().yz_ + 1;
    // 阴历月日
    int lunarMon = pCalendar->get_lunnar().date_.mon_;
    int lunarDay = pCalendar->get_lunnar().date_.day_;
    // 时辰数
    int hournum = pCalendar->get_sz().hz_ + 1;

    int sum = zhinum + lunarDay + lunarMon + hournum;
    jushu_ = sum % 9;
    if (jushu_ == 0) {
        jushu_ = 9;
    }

    // 判断阴阳遁
    // 1.冬至时间
    CDateTime dz;
    dz.date_.year_ = datetime.date_.year_;
    dz.date_.mon_ = 12;
    dz.date_.day_ = 12;
    dz.time_.h_ = 12;
    pCalendar->set_datetime(dz);
    dz.date_.day_ = pCalendar->get_jie().jq[3].dt_.date_.day_;
    dz.time_.h_ = pCalendar->get_jie().jq[3].dt_.time_.h_;
    dz.time_.m_ = pCalendar->get_jie().jq[3].dt_.time_.m_;
    dz.time_.s_ = pCalendar->get_jie().jq[3].dt_.time_.s_;

    // 2.夏至时间
    CDateTime xz;
    xz.date_.year_ = datetime.date_.year_;
    xz.date_.mon_ = 6;
    xz.date_.day_ = 15;
    xz.time_.h_ = 12;
    pCalendar->set_datetime(xz);
    xz.date_.day_ = pCalendar->get_jie().jq[3].dt_.date_.day_;
    xz.time_.h_ = pCalendar->get_jie().jq[3].dt_.time_.h_;
    xz.time_.m_ = pCalendar->get_jie().jq[3].dt_.time_.m_;
    xz.time_.s_ = pCalendar->get_jie().jq[3].dt_.time_.s_;

    // ------- 阳遁 ------- 夏至 ------- 阴遁 ------- 冬至 ------- 阳遁
    if (pCalendar->get_sec_by_date(datetime, xz) > 0 && pCalendar->get_sec_by_date(datetime, dz) <= 0) {
        // 夏至后，冬至前为阴遁
        is_yin_ = true;
    } else {
        is_yin_ = false;
    }
    CCalenderFactory::free(pCalendar);
}

}   // namespace cppbox
