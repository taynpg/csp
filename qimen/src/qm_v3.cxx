#include "qm_v3.h"

namespace csp {

QimenV3::QimenV3()
{
}

QimenV3::~QimenV3()
{
}

bool QimenV3::generate(const DateTime& dt, int ju, CalendarType ct)
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

bool QimenV3::cal_ju(const DateTime& dt)
{
    if (data_.ju != 0) {
        data_.yuan = 0;
        return true;
    }

    int jiazi = Qimen::get_jiazi_index(cal_->get_ganzhi().dg, cal_->get_ganzhi().dz);
    //  符头只有两个，甲 或者 己
    int ft = jiazi - jiazi % 5;
    // 看地支
    // 0 3 6 9  上元
    // 2 5 8 11 中元
    // 1 4 7 10 下元
    int zi = ft % 12;

    //  直接找出当月的节气信息，看在哪个节气的后面
    auto jiea = cal_->get_jq_info()[2].dt;
    auto jieb = cal_->get_jq_info()[3].dt;

    auto df1 = cal_->calsecs_by_all(dt_, jiea);
    auto df2 = cal_->calsecs_by_all(dt_, jieb);

    int r = 0;
    if (df1 >= 0 && df2 < 0) {
        data_.jieq = cal_->get_jq_info()[2].index;
    } else if (df1 < 0) {
        data_.jieq = Calendar::mod(24, cal_->get_jq_info()[2].index - 1);
    } else {
        data_.jieq = cal_->get_jq_info()[3].index;
    }
    r = juqi_[data_.jieq];
    data_.is_yin = (r % 10 == 1 ? false : true);

    if (zi % 3 == 0) {
        data_.yuan = 3;
        data_.ju = r / 1000;
    }
    if (zi % 3 == 1) {
        data_.yuan = 1;
        data_.ju = (r / 10) % 10;
    }
    if (zi % 3 == 2) {
        data_.yuan = 2;
        data_.ju = (r / 100) % 10;
    }

    return true;
}
}   // namespace csp