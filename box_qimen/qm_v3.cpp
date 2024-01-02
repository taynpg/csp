//
// Created by taynpg on 2023/12/16.
//
#include "qm_v3.h"

bool cppbox::CQimenV3::Run(const QiParam& info, CalendarType type) {
    if (!BaseRun(info, type)) {
        return false;
    }

    m_pCal->setDateTime(m_datetime);

    if (info.nJu == 0) {
        int jia = CQimen::getJiaziIndex(m_pCal->getSizhu().m_nDGan,
                                        m_pCal->getSizhu().m_nDZhi);
        //  符头只有两个，甲 或者 己
        int futou = jia - jia % 5;
        // 看地支
        // 0 3 6 9  上元
        // 2 5 8 11 中元
        // 1 4 7 10 下元
        int zhiIndex = futou % 12;
        //  直接找出当月的节气信息，看在哪个节气的后面
        const CDateTime& JieA = m_pCal->getJieFirst().datetime;
        const CDateTime& JieB = m_pCal->getJieSecond().datetime;

        // --- 节气1 --- 时间 --- 节气2 ----
        long long diffA = m_pCal->getSecondByTwoDateTime(m_datetime, JieA);
        long long diffB = m_pCal->getSecondByTwoDateTime(m_datetime, JieB);

        int nResult = 0;
        if (diffA >=0 && diffB < 0) {
            m_nJieQi = m_pCal->getJieFirst().index;
            nResult = m_nJuQi[m_nJieQi];
        }
        else if (diffA < 0) {
            m_nJieQi = CCalenderBase::getRemainder(
                24, m_pCal->getJieFirst().index - 1);
            nResult = m_nJuQi[m_nJieQi];
        }
        else {
            m_nJieQi = m_pCal->getJieSecond().index;
            nResult = m_nJuQi[m_pCal->getJieSecond().index];
        }
        if ((nResult % 10) == 1) {
            m_isYinDun = false;
        } else {
            m_isYinDun = true;
        }
        if (zhiIndex % 3 == 0) {
            m_nYuan = 3;
            m_nJushu = nResult / 1000;
        }
        if (zhiIndex % 3 == 1) {
            m_nYuan = 1;
            m_nJushu = (nResult / 10) % 10;
        }
        if (zhiIndex % 3 == 2) {
            m_nYuan = 2;
            m_nJushu = (nResult / 100) % 10;
        }
    } else {
        m_nJushu = std::abs(info.nJu);
        if ((m_nJushu - 9) > 0) {
            return false;
        }
        if (info.nJu < 0) {
            m_isYinDun = true;
        } else {
            m_isYinDun = false;
        }
        m_nYuan = 0;
    }

    genDiPan();
    genZhi();
    genJiuXing();
    genBaMen();
    genBaShen();
    genTianPan();
    genOther();

    return true;
}
