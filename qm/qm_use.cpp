#include "qm_use.h"
using namespace cppbox;

bool CQimenUse::Run(const CMDParam& param)
{
    QiParam info;
    info.datetime.m_date.m_nYear = param.year;
    info.datetime.m_date.m_nMon = param.mon;
    info.datetime.m_date.m_nDay = param.day;
    info.datetime.m_time.m_nHour = param.hour;
    info.datetime.m_time.m_nMin = param.min;
    info.datetime.m_time.m_nSec = param.sec;

    info.nJu = param.nJu;
    CQimen* qm{};

    switch (param.nType) {
    case 1:
    {
        qm = CQimenFactory::createInstance(cppbox::SHIJIA_ZHUANPAN_CHAOJIE_ZHIRUN);
        break;
    }
    case 2:
    {
        qm = CQimenFactory::createInstance(cppbox::SHIJIA_ZHUANPAN_YINPAN);
        break;
    }
    default:
        return false;
    }

    qm->BaseInit();
    if (!qm->Run(info, cppbox::CALENDAR_V1)) {
        return false;
    }
    m_print.Run(qm, param.nType);

    CQimenFactory::freeInstance(qm);
    return true;
}