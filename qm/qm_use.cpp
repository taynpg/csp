#include "qm_use.h"
#include <iostream>

using namespace cppbox;

bool CQimenUse::Run(const CMDParam& param) 
{
    QiParam info;

    if (param.isAutoDate) {
        CCalenderBase::getNowDateTime(info.datetime);
    }
    else {
        info.datetime = param.datetime;
    }

    info.nJu = param.nJu;
    CQimen* qm{};

    switch (param.nType) {
        case 1: {
            qm = CQimenFactory::createInstance(
                cppbox::SHIJIA_ZHUANPAN_CHAOJIE_ZHIRUN);
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

    qm->BaseInit();
    CalendarType tyep = (CalendarType)param.calendar_type;
    if (!qm->Run(info, tyep)) {
        std::cout << qm->getLastError() << std::endl;
        return false;
    }
    m_print.Run(qm, param.nType);

    CQimenFactory::freeInstance(qm);
    return true;
}
