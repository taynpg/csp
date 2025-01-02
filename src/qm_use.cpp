#include "qm_use.h"
#include <iostream>

using namespace cppbox;

bool CQimenUse::run(const CMDParam& param)
{
    QiParam info;

    if (param.is_auto_date_) {
        CCalender::now(info.datetime_);
    } else {
        info.datetime_ = param.dt_;
    }

    info.ju_ = param.ju_;

    std::shared_ptr<CQimen> qm = nullptr;
    switch (param.type_) {
    case 1: {
        qm = CQimenFactory::createInstance(QIMEN_STYLE::SHIJIA_ZHUANPAN_CHAOJIE_ZHIRUN);
        break;
    }
    case 2: {
        qm = CQimenFactory::createInstance(QIMEN_STYLE::SHIJIA_ZHUANPAN_YINPAN);
        break;
    }
    case 3: {
        qm = CQimenFactory::createInstance(QIMEN_STYLE::SHIJIA_ZHUANPAN_CHAIBU);
        break;
    }
    case 4: {
        qm = CQimenFactory::createInstance(QIMEN_STYLE::SHIJIA_ZHUANPAN_MAOSHAN);
        break;
    }
    default:
        return false;
    }

    qm->base_init();
    CalendarType type = (CalendarType)param.cal_type_;

    CDateTime tdt = info.datetime_;
    if (param.zone_ != 0) {
        auto pt = CCalenderFactory::create_instance(type);
        pt->get_diff_sec(info.datetime_, tdt, param.zone_ * 3600);
    }
    info.datetime_ = tdt;

    if (!qm->run(info, type)) {
        std::cout << qm->getLastError() << std::endl;
        return false;
    }
    print_.run(qm, param.type_);
    return true;
}
