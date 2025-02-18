#include "qm_v4.h"

namespace csp {

QimenV4::QimenV4()
{
}

QimenV4::~QimenV4()
{
}

bool QimenV4::generate(const DateTime& dt, int ju, CalendarType ct)
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

bool QimenV4::cal_ju(const DateTime& dt)
{
    auto jiea = cal_->get_jq_info()[2].dt;
    auto jieb = cal_->get_jq_info()[3].dt;

    // --- 节气1 --- 时间 --- 节气2 ----
    auto df1 = cal_->calsecs_by_all(dt_, jiea);
    auto df2 = cal_->calsecs_by_all(dt_, jieb);

    JieQi jieqi;
    if (df1 < 0) {
        jieqi = cal_->get_jq_info()[1];
    } else if (df1 >= 0 && df2 < 0) {
        jieqi = cal_->get_jq_info()[2];
    } else {
        jieqi = cal_->get_jq_info()[3];
    }
    data_.yuan = cal_yuan(jieqi.dt, dt_);
    data_.jieq = jieqi.index;

    if (data_.ju != 0) {
        return true;
    }

    int r = juqi_[data_.jieq];
    data_.is_yin = (r % 10 == 1 ? false : true);

    if (data_.yuan == 3) {
        data_.ju = r / 1000;
    } else if (data_.yuan == 2) {
        data_.ju = (r / 100) % 10;
    } else {
        data_.ju = (r / 10) % 10;
    }
    return true;
}

int QimenV4::cal_yuan(const DateTime& jie, const DateTime& dt)
{
    constexpr CSPLT five_day = 3600 * 24 * 5;
    // 判断是否在节气后的5天内
    auto v1 = cal_->cal_new_date(jie, five_day);
    auto dif = cal_->calsecs_by_all(dt, v1);
    if (dif < 0) {
        return 3;
    }
    // 判断是否在节气后的10天内
    auto v2 = cal_->cal_new_date(jie, five_day * 2);
    dif = cal_->calsecs_by_all(dt, v2);
    if (dif < 0) {
        return 2;
    } else {
        return 1;
    }
}

}   // namespace csp