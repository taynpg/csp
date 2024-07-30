#include "qm_use.h"
#include <iostream>

using namespace cppbox;

bool CQimenUse::run(const CMDParam& param)
{
    QiParam info;

    if (param.is_auto_date_) {
        CCalenderBase::now(info.datetime_);
    } else {
        info.datetime_ = param.dt_;
    }

    info.ju_ = param.ju_;
    CQimen* qm{};

    switch (param.type_) {
        case 1: {
            qm = CQimenFactory::createInstance(cppbox::SHIJIA_ZHUANPAN_CHAOJIE_ZHIRUN);
            break;
        }
        case 2: {
            qm = CQimenFactory::createInstance(cppbox::SHIJIA_ZHUANPAN_YINPAN);
            break;
        }
        case 3: {
            qm = CQimenFactory::createInstance(cppbox::SHIJIA_ZHUANPAN_CHAIBU);
            break;
        }
        default:
            return false;
    }

    qm->base_init();
    CalendarType type = (CalendarType)param.cal_type_;
    if (!qm->run(info, type)) {
        std::cout << qm->getLastError() << std::endl;
        return false;
    }
    print_.run(qm, param.type_);
    CQimenFactory::freeInstance(qm);
    return true;
}
