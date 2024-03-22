#include "qm_v1.h"

#include <cmath>

namespace cppbox {

CQiMenV1::CQiMenV1()
{
    m_pCal = nullptr;
    m_nJushu = 0;
    m_calType = CALENDAR_V1;

    // 下标 0 指小寒，即从小寒开始向后排列。
    m_nJuQi[0] = 2851;
    m_nJuQi[1] = 3961;
    m_nJuQi[2] = 8521;
    m_nJuQi[3] = 9631;
    m_nJuQi[4] = 1741;
    m_nJuQi[5] = 3961;
    m_nJuQi[6] = 4171;
    m_nJuQi[7] = 5281;
    m_nJuQi[8] = 4171;
    m_nJuQi[9] = 5281;
    m_nJuQi[10] = 6391;
    m_nJuQi[11] = 9360;
    m_nJuQi[12] = 8250;
    m_nJuQi[13] = 7140;
    m_nJuQi[14] = 2580;
    m_nJuQi[15] = 1470;
    m_nJuQi[16] = 9360;
    m_nJuQi[17] = 7140;
    m_nJuQi[18] = 6930;
    m_nJuQi[19] = 5820;
    m_nJuQi[20] = 6930;
    m_nJuQi[21] = 5820;
    m_nJuQi[22] = 4710;
    m_nJuQi[23] = 1741;

    m_wuBuYu[0] = 6;
    m_wuBuYu[1] = 17;
    m_wuBuYu[2] = 28;
    m_wuBuYu[3] = 39;
    m_wuBuYu[4] = 50;
    m_wuBuYu[5] = 1;
    m_wuBuYu[6] = 12;
    m_wuBuYu[7] = 33;
    m_wuBuYu[8] = 44;
    m_wuBuYu[9] = 55;
}

CQiMenV1::~CQiMenV1()
{
    if (!m_pOneYear.empty()) {
        std::vector<OneDay*>::iterator it;
        for (it = m_pOneYear.begin(); it != m_pOneYear.end(); ++it) {
            delete *it;
        }
    }
}

// 传入日期数据
bool CQiMenV1::Run(const QiParam& info, CalendarType type)
{
    if (!BaseRun(info, type)) {
        return false;
    }

    if (info.nJu == 0) {
        inferenceDate();
        // printYearDay();

        OneDay* pToday = searchDay(info.datetime);
        if (!pToday) {
            return false;
        }

        m_nJiaZi = pToday->nGanZhiIndex;
        m_nJieQi = pToday->nJieIndex;

        int nResult = m_nJuQi[pToday->nJieIndex];
        if ((nResult % 10) == 1) {
            m_isYinDun = false;
        } else {
            m_isYinDun = true;
        }

        int num = (pToday->nGanZhiIndex % 15) / 5;
        if (num == 0) {
            m_nYuan = 3;
            m_nJushu = nResult / 1000;
        } else if (num == 1) {
            m_nYuan = 2;
            m_nJushu = (nResult / 100) % 10;
        } else {
            m_nYuan = 1;
            m_nJushu = (nResult / 10) % 10;
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

void CQiMenV1::getCurrentMonJie(const CDateTime& datetime, CDateTime& datetimeJie, int& nJiazi, int& nFutouDiff)
{
    m_pCal->setDateTime(datetime);
    datetimeJie = m_pCal->getJieFirst().datetime;
    m_pCal->setDateTime(datetimeJie);
    nJiazi = getJiaziIndex(m_pCal->getSizhu().m_nDGan, m_pCal->getSizhu().m_nDZhi);
    int nFutou = (nJiazi / 15) * 15;
    nFutouDiff = nJiazi - nFutou;
    if (nFutouDiff < 0) {
        nFutouDiff = -nFutouDiff;
    }
}

// 推算一年的日历
void CQiMenV1::inferenceDate()
{
    int nTem = 0;
    int nStartJie = 22;
    int nJiazi = 0;
    int nFutouDiff = 0;

    CDateTime dateDaxuePreYear(m_datetime.m_date.m_nYear - 1, 12, 1, 12, 0, 0);
    CDateTime dateDaxue(m_datetime.m_date.m_nYear, 12, 1, 12, 0, 0);
    CDateTime dateMangzhong(m_datetime.m_date.m_nYear, 6, 6, 12, 0, 0);
    CDateTime dateTem;

    CDateTime dateJieTime;
    CDateTime cycleDatetime;
    getCurrentMonJie(dateDaxuePreYear, dateJieTime, nJiazi, nFutouDiff);
    cycleDatetime = dateJieTime;
    // 保存大雪的三元
    savePart(cycleDatetime.m_date, nStartJie, nJiazi, -1, 15 - nFutouDiff);
    if (nFutouDiff < 9) {
        nStartJie = cb::getRemainder(24, ++nStartJie);
    }
    // 保存大雪到芒种
    savePart(cycleDatetime.m_date, nStartJie, nJiazi, 10, 300);
    getCurrentMonJie(dateMangzhong, dateJieTime, nTem, nFutouDiff);

    // 递增日期此时存的是最后一个日期的下一个日期，所以这里算差值要回退一天
    dateTem = CDateTime(cycleDatetime.m_date);
    m_pCal->getPreDay(dateTem);
    int nDayDiff = m_pCal->getDiffByTwoDate(dateTem.m_date, dateJieTime.m_date);
    nDayDiff = nDayDiff < 0 ? -nDayDiff : nDayDiff;
    // 保存芒种三元
    savePart(cycleDatetime.m_date, nStartJie, nJiazi, -1, 14);
    if (nDayDiff < 9) {
        nStartJie = cb::getRemainder(24, ++nStartJie);
    }
    // 保存芒种到大雪
    savePart(cycleDatetime.m_date, nStartJie, nJiazi, 22, 300);
    getCurrentMonJie(dateDaxue, dateJieTime, nTem, nFutouDiff);
    dateTem = CDateTime(cycleDatetime.m_date);
    // 递增日期此时存的是最后一个日期的下一个日期，所以这里算差值要回退一天
    m_pCal->getPreDay(dateTem);
    nDayDiff = m_pCal->getDiffByTwoDate(dateTem.m_date, dateJieTime.m_date);
    nDayDiff = nDayDiff < 0 ? -nDayDiff : nDayDiff;
    // 保存大雪的三元
    savePart(cycleDatetime.m_date, nStartJie, nJiazi, -1, 14);
    if (nDayDiff < 9) {
        nStartJie = cb::getRemainder(24, ++nStartJie);
    }
    // 保存大雪到立春
    savePart(cycleDatetime.m_date, nStartJie, nJiazi, -1, 60);
}

void CQiMenV1::savePart(CDate& date, int& nUpper, int& nJiazi, int nPur, int nDays)
{
    int nCycle = 0;
    nUpper = cb::getRemainder(24, nUpper);
    for (int i = 0; i < nDays; ++i, ++nCycle) {
        if (nCycle == 15) {
            nUpper = cb::getRemainder(24, ++nUpper);
            if (nUpper == nPur) {
                saveDay(date, nUpper, nJiazi);
                m_pCal->getNextDay(date);
                nJiazi = cb::getRemainder(60, ++nJiazi);
                break;
            }
            nCycle = 0;
        }
        saveDay(date, nUpper, nJiazi);
        m_pCal->getNextDay(date);
        nJiazi = cb::getRemainder(60, ++nJiazi);
    }
}

void CQiMenV1::saveDay(const CDate& date, int nJie, int& nJiazi)
{
    auto* pDay = new OneDay();
    pDay->nGanZhiIndex = nJiazi;
    pDay->nJieIndex = nJie;
    pDay->date = date;
    m_pOneYear.push_back(pDay);
}

// 在一年的日历中查找当天的信息
OneDay* CQiMenV1::searchDay(const CDateTime& datetime)
{
    OneDay*                              p = nullptr;
    std::vector<OneDay*>::const_iterator it;
    for (it = m_pOneYear.begin(); it != m_pOneYear.end(); ++it) {
        if ((*it)->date == datetime.m_date) {
            p = *it;
            break;
        }
    }
    return p;
}

// 打印所推演的一年日历
/*void CQiMenV1::printYearDay() {

    std::vector<OneDay *>::const_iterator it;
    for (it = m_pOneYear.begin(); it != m_pOneYear.end(); ++it) {

        std::printf("%04d-%02d-%02d %02d %02d\n" , (*it)->date.m_nYear,
(*it)->date.m_nMon,
        (*it)->date.m_nDay, (*it)->nJieIndex, (*it)->nGanZhiIndex);
    }
}*/

// 获取给定甲子的两个寻空
void CQiMenV1::getXunKong(int nJiazi, int& nKongA, int& nKongB)
{
    nKongA = ((nJiazi / 10) * 10 + 10) % 12;
    nKongB = ((nJiazi / 10) * 10 + 11) % 12;
}

// 排地盘
void CQiMenV1::genDiPan()
{
    // 所谓的几局，就是甲子戊居于第几宫
    int nStartIndex = cb::getRemainder(9, m_nJushu - 1);
    // 转成定义位置
    int nPosition = m_nGuaNum2Pos[nStartIndex];
    // 甲子戊
    m_nDiPan[nPosition] = 4;
    int nk{};
    if (m_isYinDun) {
        nk = -1;
    } else {
        nk = 1;
    }
    for (int i = 0; i < 5; ++i) {
        m_nDiPan[m_nGuaNum2Pos[cb::getRemainder(9, nStartIndex += nk)]] = 5 + i;
    }
    for (int i = 0; i < 3; ++i) {
        m_nDiPan[m_nGuaNum2Pos[cb::getRemainder(9, nStartIndex += nk)]] = 3 - i;
    }

    // 判断是否是 五不遇时
    int hg = m_pCal->getSizhu().m_nHGan;
    int hz = m_pCal->getSizhu().m_nHZhi;
    if (m_wuBuYu[m_pCal->getSizhu().m_nDGan] == CQimen::getJiaziIndex(hg, hz)) {
        m_isWuBuYu = true;
    } else {
        m_isWuBuYu = false;
    }
}
// 查找值符，值使
void CQiMenV1::genZhi()
{
    // 六十甲子
    int hindex = getJiaziIndex(m_pCal->getSizhu().m_nHGan, m_pCal->getSizhu().m_nHZhi);
    // 查看旬头遁甲位置
    int ganIndex = (hindex / 10) + 4;
    // 这里根据需要搜寻所在地盘的宫位在哪里
    // 这里算出来的值是场地位置
    m_nZhiFuPos = getIndex(m_nDiPan, 9, ganIndex);
    m_nZhiShiPos = m_nZhiFuPos;
}
// 排九星
void CQiMenV1::genJiuXing()
{
    // 先查验当前的局是否为伏吟
    if (m_pCal->getSizhu().m_nHGan == 0) {
        for (int i = 0; i < 9; ++i) {
            m_JiuXingRe[i] = m_JiuXingPre[i];
            m_nBamenRe[i] = m_nBamenPre[i];
        }
        return;
    }
    // 时辰天干在地盘的位置
    int nstart = getIndex(m_nDiPan, 9, m_pCal->getSizhu().m_nHGan);
    // 如果这个位置在中宫，那么从寄宫开始
    if (nstart == 8) {
        nstart = m_nJiGongPos;
    }
    int nxing = m_JiuXingPre[m_nZhiFuPos];
    // 如果当前的值符为天禽星，那么也归为原寄宫星
    if (nxing == 4) {
        nxing = m_JiuXingPre[m_nJiGongPos];
    }
    m_JiuXingRe[nstart] = nxing;
    // 查找当前的九星的起始相对位置
    int nturn = getIndex(m_JiuXingTurn, 8, nxing);
    for (int i = 0; i < 8; ++i) {
        m_JiuXingRe[cb::getRemainder(8, --nstart)] = m_JiuXingTurn[cb::getRemainder(8, ++nturn)];
    }
}
// 排八门
void CQiMenV1::genBaMen()
{
    // 查看当前旬头六仪
    int nJiazi = getJiaziIndex(m_pCal->getSizhu().m_nHGan, m_pCal->getSizhu().m_nHZhi);
    // 查看当前时辰距离符头有几跳
    int ndiff = nJiazi - (nJiazi / 10) * 10;

    int nk = 1;
    if (m_isYinDun) {
        nk = -1;
    }

    // m_nBamenPre 中仅有 8 个数据，当值符值使位置在中宫8的时候，转成寄宫
    int startPos = m_nZhiShiPos;
    if (startPos == 8) {
        startPos = m_nJiGongPos;
    }

    // m_nPos2GuaNum 用于位置转卦序
    int guaIndex = m_nPos2GuaNum[startPos];

    for (int i = 0; i < ndiff; ++i) {
        guaIndex = cb::getRemainder(9, guaIndex += nk);
    }
    //
    int nPos = m_nGuaNum2Pos[guaIndex];

    // 如果门转到了中宫，也是按照寄宫处理。
    if (nPos == 8) {
        nPos = m_nJiGongPos;
    }

    int menIndex = getIndex(m_nBamenTurn, 8, m_nBamenPre[startPos]);
    for (int i = 0; i < 8; ++i, nPos--, ++menIndex) {
        m_nBamenRe[cb::getRemainder(8, nPos)] = m_nBamenTurn[cb::getRemainder(8, menIndex)];
    }
}
// 排八神
void CQiMenV1::genBaShen()
{
    int nxing = m_JiuXingPre[m_nZhiFuPos];
    if (nxing == 4) {
        nxing = m_JiuXingPre[m_nJiGongPos];
    }
    // 看看值符被转到哪里了
    int sIndex = getIndex(m_JiuXingRe, 9, nxing);
    int nk = 1;
    if (!m_isYinDun) {
        nk = -1;
    }
    for (int i = 0; i < 9; ++i, sIndex += nk) {
        if (i == 4) {
            ++i;
        }
        m_nBaShenRe[cb::getRemainder(8, sIndex)] = i;
    }
}
// 排天盘
void CQiMenV1::genTianPan()
{
    // 当前九星宫位的天盘就是：当前星原来宫位中的地盘
    for (int i = 0; i < 8; ++i) {
        m_nTianPan[i] = m_nDiPan[m_nGuaNum2Pos[m_JiuXingRe[i]]];
    }
}
// 排旬空马星
void CQiMenV1::genOther()
{
    getXunKong(getJiaziIndex(m_pCal->getSizhu().m_nYGan, m_pCal->getSizhu().m_nYZhi), m_nXunKong[0], m_nXunKong[1]);
    getXunKong(getJiaziIndex(m_pCal->getSizhu().m_nMGan, m_pCal->getSizhu().m_nMZhi), m_nXunKong[2], m_nXunKong[3]);
    getXunKong(getJiaziIndex(m_pCal->getSizhu().m_nDGan, m_pCal->getSizhu().m_nDZhi), m_nXunKong[4], m_nXunKong[5]);
    getXunKong(getJiaziIndex(m_pCal->getSizhu().m_nHGan, m_pCal->getSizhu().m_nHZhi), m_nXunKong[6], m_nXunKong[7]);

    m_nKongWang[0] = m_dizhi[m_nXunKong[6]];
    m_nKongWang[1] = m_dizhi[m_nXunKong[7]];

    // 马星
    int nchongzhi = m_zhichong[m_sanhe[m_pCal->getSizhu().m_nHZhi]];
    m_nMaXing = m_dizhi[nchongzhi];
}
}   // namespace cppbox
