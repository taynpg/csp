#include "qm_v2.h"

namespace cppbox {

CQimenV2::CQimenV2() {
    m_pCal = nullptr;
    m_nJushu = 0;
    m_calType = CALENDAR_V1;
}

CQimenV2::~CQimenV2() { CCalenderFactory::freeCalender(m_pCal); }

bool CQimenV2::Run(const QiParam& info, CalendarType type) {
    if (!BaseRun(info, type)) {
        return false;
    }
    if (info.nJu == 0) {
        getJushu(info.datetime);
    }

    // 这里跑一下今天的日期以供后续使用
    m_pCal->setDateTime(m_datetime);

    genDiPan();
    genZhi();
    genJiuXing();
    genBaMen();
    genBaShen();
    genTianPan();
    genOther();

    return true;
}

void CQimenV2::getJushu(const CDateTime& datetime) {
    CCalenderBase* pCalendar = CCalenderFactory::creatInstance(m_calType);
    pCalendar->setDateTime(datetime);
    // 年支数
    int zhinum = pCalendar->getSizhu().m_nYZhi + 1;
    // 阴历月日
    int lunarMon = pCalendar->getLunarDateTime().m_date.m_nMon;
    int lunarDay = pCalendar->getLunarDateTime().m_date.m_nDay;
    // 时辰数
    int hournum = pCalendar->getSizhu().m_nHZhi + 1;

    int sum = zhinum + lunarDay + lunarMon + hournum;
    m_nJushu = sum % 9;
    if (m_nJushu == 0) {
        m_nJushu = 9;
    }

    // 判断阴阳遁
    // 1.冬至时间
    CDateTime dz;
    dz.m_date.m_nYear = datetime.m_date.m_nYear;
    dz.m_date.m_nMon = 12;
    dz.m_date.m_nDay = 12;
    dz.m_time.m_nHour = 12;
    pCalendar->setDateTime(dz);
    dz.m_date.m_nDay = pCalendar->getJieSecond().datetime.m_date.m_nDay;
    dz.m_time.m_nHour = pCalendar->getJieSecond().datetime.m_time.m_nHour;
    dz.m_time.m_nMin = pCalendar->getJieSecond().datetime.m_time.m_nMin;
    dz.m_time.m_nSec = pCalendar->getJieSecond().datetime.m_time.m_nSec;

    // 2.夏至时间
    CDateTime xz;
    xz.m_date.m_nYear = datetime.m_date.m_nYear;
    xz.m_date.m_nMon = 6;
    xz.m_date.m_nDay = 15;
    xz.m_time.m_nHour = 12;
    pCalendar->setDateTime(xz);
    xz.m_date.m_nDay = pCalendar->getJieSecond().datetime.m_date.m_nDay;
    xz.m_time.m_nHour = pCalendar->getJieSecond().datetime.m_time.m_nHour;
    xz.m_time.m_nMin = pCalendar->getJieSecond().datetime.m_time.m_nMin;
    xz.m_time.m_nSec = pCalendar->getJieSecond().datetime.m_time.m_nSec;

    // ------- 阳遁 ------- 夏至 ------- 阴遁 ------- 冬至 ------- 阳遁
    if (pCalendar->getSecondByTwoDateTime(datetime, xz) > 0 &&
        pCalendar->getSecondByTwoDateTime(datetime, dz) <= 0) {
        // 夏至后，冬至前为阴遁
        m_isYinDun = true;
    } else {
        m_isYinDun = false;
    }
    CCalenderFactory::freeCalender(pCalendar);
}

}  // namespace cppbox
