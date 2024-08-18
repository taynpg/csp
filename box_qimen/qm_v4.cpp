#include "qm_v4.h"

namespace cppbox {

bool CQimenV4::run(const QiParam& info, CalendarType type)
{
    if (!base_run(info, type)) {
        return false;
    }
    pcal_->set_datetime(datetime_);

    //  直接找出当月的节气信息，看在哪个节气的后面
    const CDateTime& JieA = pcal_->first_jie().dt_;
    const CDateTime& JieB = pcal_->second_jie().dt_;

    // --- 节气1 --- 时间 --- 节气2 ----
    long long diffA = pcal_->get_sec_by_date(datetime_, JieA);
    long long diffB = pcal_->get_sec_by_date(datetime_, JieB);

    // 取节气1前的内容
    if (diffA < 0) {
        //yuan_ = get_yuan(CCalenderBase::remain(24, pcal_->first_jie().index_ - 1), datetime_);
    }
    // 取节气1,2之间的内容
    else if (diffA >= 0 && diffB < 0) {
        //yuan_ = get_yuan(CCalenderBase::remain(24, pcal_->first_jie().index_), datetime_);
    }
    // 取节气2后的内容
    else {
        //yuan_ = get_yuan(CCalenderBase::remain(24, pcal_->first_jie().index_ + 1), datetime_);
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

// 三元 0 手动, 1下，2中，3上
int CQimenV4::get_yuan(const CDateTime& jie, const CDateTime& datetime)
{
    constexpr int five_day = 3600 * 24 * 5;

    return 0;
}

}   // namespace cppbox