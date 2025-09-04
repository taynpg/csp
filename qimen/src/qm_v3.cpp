#include "qm_v3.h"

namespace csp {

QimenV3::QimenV3()
{
}

QimenV3::~QimenV3()
{
}

bool QimenV3::generate(const DateTime& dt, int ju)
{
    if (!Qimen::set_and_check(dt, ju)) {
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
    auto jiazi = Qimen::jiazi(*data_.dt_);
    //  符头只有两个，甲 或者 己
    int ft = jiazi.di - jiazi.di % 5;
    // 看地支
    // 0 3 6 9  上元
    // 2 5 8 11 中元
    // 1 4 7 10 下元
    int zi = ft % 12;

    //  直接找出当月的节气信息，看在哪个节气的后面
    int firstIndex{};
    int secondIndex{};
    auto first = Qimen::get_jq(*data_.dt_, firstIndex, false);
    auto second = Qimen::get_jq(*data_.dt_, secondIndex, true);

    auto df1 = data_.dt_->subtract(first);
    auto df2 = data_.dt_->subtract(second);

    int r = 0;
    if (df1 >= 0 && df2 < 0) {
        data_.jieq = firstIndex;
    } else if (df1 < 0) {
        data_.jieq = mod(24, firstIndex - 1);
    } else {
        data_.jieq = secondIndex;
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