#include "qm_use.h"
using namespace cppbox;

bool CQimenUse::Run(const CMDParam& param)
{
    QiInfomation info;
    info.datetime.m_date.m_nYear = param.year;
    info.datetime.m_date.m_nMon = param.mon;
    info.datetime.m_date.m_nDay = param.day;
    info.datetime.m_time.m_nHour = param.hour;
    info.datetime.m_time.m_nMin = param.min;
    info.datetime.m_time.m_nSec = param.sec;

    info.nJu = param.nJu;

    CQimen* qm = CQimenFactory::createInstance(cppbox::SHIJIA_ZHUANPAN_CHAOJIE_ZHIRUN);
    qm->Init();
    if (qm->Run(info, cppbox::CALENDAR_V1)) {
        m_print.Run(qm);
    }
    else {
        return false;
    }
    return true;
}