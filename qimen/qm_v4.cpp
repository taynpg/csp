#include "qm_v4.h"
#include <cmath>

namespace cppbox {

bool CQimenV4::run(const QiParam& info, CalendarType type)
{
    if (!base_run(info, type)) {
        return false;
    }
    pcal_->set_datetime(datetime_);

    //  直接找出当月的节气信息，看在哪个节气的后面 
    const CDateTime& JieA = pcal_->get_jie().jq[2].dt_;
    const CDateTime& JieB = pcal_->get_jie().jq[3].dt_;

    // --- 节气1 --- 时间 --- 节气2 ----
    long long diffA = pcal_->get_sec_by_date(datetime_, JieA);
    long long diffB = pcal_->get_sec_by_date(datetime_, JieB);

    CJieQi jie;
    // 取节气1前的内容
    if (diffA < 0) {
        jie = pcal_->get_jie().jq[1];
        yuan_ = get_yuan(jie.dt_, datetime_);
    }
    // 取节气1,2之间的内容
    else if (diffA >= 0 && diffB < 0) {
        jie = pcal_->get_jie().jq[2];
        yuan_ = get_yuan(jie.dt_, datetime_);
    }
    // 取节气2后的内容
    else {
        jie = pcal_->get_jie().jq[3];
        yuan_ = get_yuan(jie.dt_, datetime_);
    }

    // 定局数
    if (info.ju_ == 0) {
        int nResult = ju_qi_[jie.index_];
        if ((nResult % 10) == 1) {
            is_yin_ = false;
        } else {
            is_yin_ = true;
        }
        if (yuan_ == 3) {
            jushu_ = nResult / 1000;
        } else if (yuan_ == 2) {
            jushu_ = (nResult / 100) % 10;
        } else {
            jushu_ = (nResult / 10) % 10;
        }
    } else {
        jushu_ = std::abs(info.ju_);
        if ((jushu_ - 9) > 0) {
            return false;
        }
        if (info.ju_ < 0) {
            is_yin_ = true;
        } else {
            is_yin_ = false;
        }
    }

    jq_ = jie.index_;

    gen_dipan();
    gen_zhi();
    gen_jx();
    gen_bm();
    gen_bs();
    gen_tp();
    gen_other();

    return true;
}

// 三元 0 手动, 1下，2中，3上
int CQimenV4::get_yuan(const CDateTime& jie, const CDateTime& datetime)
{
    constexpr int five_day = 3600 * 24 * 5;

    CDateTime v1;
    CDateTime v2;
    // 判断是否在节气后的5天内
    pcal_->get_diff_sec(jie, v1, five_day);
    long long diff = pcal_->get_sec_by_date(datetime, v1);
    if (diff < 0) {
        return 3;
    }
    // 判断是否在节气后的10天内
    pcal_->get_diff_sec(jie, v2, five_day * 2);
    diff = pcal_->get_sec_by_date(datetime, v2);
    if (diff < 0) {
        return 2;
    } else {
        return 1;
    }
}
}   // namespace cppbox
