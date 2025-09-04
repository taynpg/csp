#include "qm_v4.h"

namespace csp {

QimenV4::QimenV4()
{
}

QimenV4::~QimenV4()
{
}

bool QimenV4::generate(const DateTime& dt, int ju)
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

bool QimenV4::cal_ju(const DateTime& dt)
{
    int firstIndex{};
    int secondIndex{};
    int preIndex{};
    auto first = Qimen::get_jq(*data_.dt_, firstIndex, false);
    auto second = Qimen::get_jq(*data_.dt_, secondIndex, true);
    auto df1 = data_.dt_->subtract(first);
    auto df2 = data_.dt_->subtract(second);

    auto preMon = tyme::SolarMonth::from_ym(data_.dt_->get_year(), data_.dt_->get_month());
    preMon = preMon.next(-1);
    auto preDt =
        std::make_shared<tyme::SolarTime>(preMon.get_year(), preMon.get_month(), data_.dt_->get_day(),
                                          data_.dt_->get_hour(), data_.dt_->get_minute(), data_.dt_->get_second());
    auto preJq = Qimen::get_jq(*preDt, preIndex, true);

    tyme::SolarTime* reTime{};
    if (df1 < 0) {
        reTime = &preJq;
        data_.jieq = preIndex;
    } else if (df1 >= 0 && df2 < 0) {
        reTime = &first;
        data_.jieq = firstIndex;
    } else {
        reTime = &second;
        data_.jieq = secondIndex;
    }
    data_.yuan = cal_yuan(reTime);
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

int QimenV4::cal_yuan(tyme::SolarTime* dt)
{
    constexpr uint32_t five_day = 3600 * 24 * 5;
    constexpr uint32_t ten_day = five_day * 2;
    // 判断是否在节气后的5天内

    auto diff = data_.dt_->subtract(*dt);
    if (diff - five_day < 0) {
        return 3;
    }
    // 判断是否在节气后的10天内
    if (diff - ten_day < 0) {
        return 2;
    } else {
        return 1;
    }
}

}   // namespace csp