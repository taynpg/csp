//
// Created by taynpg on 2023/12/16.
//
#include "qm_v3.h"
#include <cmath>

bool cppbox::CQimenV3::run(const QiParam& info, CalendarType type)
{
    if (!base_run(info, type)) {
        return false;
    }

    pcal_->set_datetime(datetime_);

    if (info.ju_ == 0) {
        int jia = CQimen::get_jz(pcal_->get_sz().dg_, pcal_->get_sz().dz_);
        //  符头只有两个，甲 或者 己
        int futou = jia - jia % 5;
        // 看地支
        // 0 3 6 9  上元
        // 2 5 8 11 中元
        // 1 4 7 10 下元
        int zhi_index = futou % 12;
        //  直接找出当月的节气信息，看在哪个节气的后面
        const CDateTime& JieA = pcal_->get_jie().jq[2].dt_;
        const CDateTime& JieB = pcal_->get_jie().jq[3].dt_;

        // --- 节气1 --- 时间 --- 节气2 ----
        long long d1 = pcal_->get_sec_by_date(datetime_, JieA);
        long long d2 = pcal_->get_sec_by_date(datetime_, JieB);

        int nr = 0;
        if (d1 >= 0 && d2 < 0) {
            jq_ = pcal_->get_jie().jq[2].index_;
            nr = ju_qi_[jq_];
        } else if (d1 < 0) {
            jq_ = CCalender::remain(24, pcal_->get_jie().jq[2].index_ - 1);
            nr = ju_qi_[jq_];
        } else {
            jq_ = pcal_->get_jie().jq[3].index_;
            nr = ju_qi_[pcal_->get_jie().jq[3].index_];
        }
        if ((nr % 10) == 1) {
            is_yin_ = false;
        } else {
            is_yin_ = true;
        }
        if (zhi_index % 3 == 0) {
            yuan_ = 3;
            jushu_ = nr / 1000;
        }
        if (zhi_index % 3 == 1) {
            yuan_ = 1;
            jushu_ = (nr / 10) % 10;
        }
        if (zhi_index % 3 == 2) {
            yuan_ = 2;
            jushu_ = (nr / 100) % 10;
        }
    } else {
        jushu_ = abs(info.ju_);
        if ((jushu_ - 9) > 0) {
            return false;
        }
        if (info.ju_ < 0) {
            is_yin_ = true;
        } else {
            is_yin_ = false;
        }
        yuan_ = 0;
    }

    gen_dipan();
    gen_zhi();
    gen_jx();
    gen_bm();
    gen_bs();
    gen_tp();
    gen_other();

    return true;
}
