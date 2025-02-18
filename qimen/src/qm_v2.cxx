#include "qm_v2.h"

namespace csp {

QimenV2::QimenV2()
{
}

QimenV2::~QimenV2()
{
}

bool QimenV2::generate(const DateTime& dt, int ju, CalendarType ct)
{
    if (!Qimen::set_and_check(dt, ju, ct)) {
        return false;
    }
    if (!cal_->generate(dt_)) {
        return false;
    }
    if (data_.ju == 0 && !cal_ju(dt)) {
        return false;
    }

    cal_dipan();
    cal_duty();
    cal_jiuxing();
    cal_bamen();
    cal_bashen();
    cal_tianpan();
    cal_other();

    return true;
}

bool QimenV2::cal_ju(const DateTime& dt)
{
    auto tcal = Calendar::instance(ctype_);
    if (!tcal->generate(dt)) {
        return false;
    }
    // 年支数字
    int yz = cal_->get_ganzhi().yz + 1;
    // 农历月日
    int lm = cal_->get_lunar().mon;
    int ld = cal_->get_lunar().day;
    // 时辰数字
    int hm = cal_->get_ganzhi().hz + 1;

    int sum = yz + lm + ld + hm;
    data_.ju = sum % 9;
    data_.ju = (data_.ju == 0 ? 9 : data_.ju);

    // 判断阴阳遁
    // *************** 冬至
    DateTime dz(dt.year, 12, 12, 12, 0, 0);
    if (!tcal->generate(dz)) {
        return false;
    }
    dz = cal_->get_jq_info()[3].dt;
    // *************** 夏至
    DateTime xz(dt.year, 6, 15, 12, 0, 0);
    if (!tcal->generate(xz)) {
        return false;
    }
    xz = cal_->get_jq_info()[3].dt;

    auto da = cal_->calsecs_by_all(dt, xz);
    auto db = cal_->calsecs_by_all(dt, dz);

    // --- 阳遁 --- 夏至 --- 阴遁 --- 冬至 --- 阳遁
    if (da > 0 && db <= 0) {
        data_.is_yin = true;
    } else {
        data_.is_yin = false;
    }
    return true;
}

}   // namespace csp
