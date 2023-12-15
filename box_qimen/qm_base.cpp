#include "qm_base.h"

#include "qm_v1.h"
#include "qm_v2.h"
#include "qm_v3.h"

namespace cppbox {

// 获取实例
CQimen* CQimenFactory::createInstance(QIMEN_STYLE type) {
    CQimen* pQimen = nullptr;
    switch (type) {
        case SHIJIA_ZHUANPAN_CHAOJIE_ZHIRUN: {  // 时家超接置润法
            pQimen = new CQiMenV1();
            break;
        }
        case SHIJIA_ZHUANPAN_YINPAN: {
            pQimen = new CQimenV2();
            break;
        }
        case SHIJIA_ZHUANPAN_CHAIBU: {
            pQimen = new CQimenV3();
            break;
        }
        default:
            break;
    }
    return pQimen;
}
// 释放实例
void CQimenFactory::freeInstance(CQimen* pQimen) { delete pQimen; }

CQimen::CQimen() : m_pCal(nullptr), m_calType(CALENDAR_V1) {
    m_sanhe[0] = 8;
    m_sanhe[1] = 5;
    m_sanhe[2] = 2;
    m_sanhe[3] = 2;
    m_sanhe[4] = 8;
    m_sanhe[5] = 5;
    m_sanhe[6] = 2;
    m_sanhe[7] = 11;
    m_sanhe[8] = 8;
    m_sanhe[9] = 5;
    m_sanhe[10] = 2;
    m_sanhe[11] = 11;

    m_zhichong[8] = 2;
    m_zhichong[5] = 11;
    m_zhichong[2] = 8;
    m_zhichong[2] = 8;
    m_zhichong[11] = 5;
}

CQimen::~CQimen() { CCalenderFactory::freeCalender(m_pCal); }

// 设置十二地支位置对应
void CQimen::setDizhi() {
    m_dizhi[0] = 7;
    m_dizhi[1] = 6;
    m_dizhi[2] = 6;
    m_dizhi[3] = 5;
    m_dizhi[4] = 4;
    m_dizhi[5] = 4;
    m_dizhi[6] = 3;
    m_dizhi[7] = 2;
    m_dizhi[8] = 2;
    m_dizhi[9] = 1;
    m_dizhi[10] = 0;
    m_dizhi[11] = 0;
}

void CQimen::setJiuXingPre() {
    m_JiuXingPre[0] = 5;
    m_JiuXingPre[1] = 6;
    m_JiuXingPre[2] = 1;
    m_JiuXingPre[3] = 8;
    m_JiuXingPre[4] = 3;
    m_JiuXingPre[5] = 2;
    m_JiuXingPre[6] = 7;
    m_JiuXingPre[7] = 0;
    m_JiuXingPre[8] = 4;
}

// 设置九星的旋转位置[相对顺序]
void CQimen::setJiuXingTurn() {
    m_JiuXingTurn[0] = 0;
    m_JiuXingTurn[1] = 7;
    m_JiuXingTurn[2] = 2;
    m_JiuXingTurn[3] = 3;
    m_JiuXingTurn[4] = 8;
    m_JiuXingTurn[5] = 1;
    m_JiuXingTurn[6] = 6;
    m_JiuXingTurn[7] = 5;
}

// 设置八门的初始位置
void CQimen::setBaMenPre() {
    m_nBamenPre[0] = 5;
    m_nBamenPre[1] = 6;
    m_nBamenPre[2] = 1;
    m_nBamenPre[3] = 8;
    m_nBamenPre[4] = 3;
    m_nBamenPre[5] = 2;
    m_nBamenPre[6] = 7;
    m_nBamenPre[7] = 0;
    m_nBamenPre[8] = 4;
}

// 设置八门的旋转位置
void CQimen::setBaMenTurn() {
    m_nBamenTurn[0] = 0;
    m_nBamenTurn[1] = 7;
    m_nBamenTurn[2] = 2;
    m_nBamenTurn[3] = 3;
    m_nBamenTurn[4] = 8;
    m_nBamenTurn[5] = 1;
    m_nBamenTurn[6] = 6;
    m_nBamenTurn[7] = 5;
}

// 设置八神的初始位置
void CQimen::setBaShenPre() {
    m_nBaShenPre[0] = 5;
    m_nBaShenPre[1] = 6;
    m_nBaShenPre[2] = 1;
    m_nBaShenPre[3] = 8;
    m_nBaShenPre[4] = 3;
    m_nBaShenPre[5] = 2;
    m_nBaShenPre[6] = 7;
    m_nBaShenPre[7] = 0;
    m_nBaShenPre[8] = 4;
}

// 设置九宫的数字对应
void CQimen::setGongBaseNum() {
    // 定义位置从 0 开始分别对应卦序
    m_nPos2GuaNum[0] = 5;
    m_nPos2GuaNum[1] = 6;
    m_nPos2GuaNum[2] = 1;
    m_nPos2GuaNum[3] = 8;
    m_nPos2GuaNum[4] = 3;
    m_nPos2GuaNum[5] = 2;
    m_nPos2GuaNum[6] = 7;
    m_nPos2GuaNum[7] = 0;
    m_nPos2GuaNum[8] = 4;
}

// 设置初始内容之后，执行此函数，对初始内容整理并生成所需的信息。
void CQimen::prepare() {
    // m_nGuaNum2Pos 就是后天八卦的顺序数和定义位置的对照。
    for (int i = 0; i < 9; ++i) {
        m_nGuaNum2Pos[m_nPos2GuaNum[i]] = i;
    }
}

// 设置寄宫
void CQimen::setJiGong(int nGong) {
    // 这里减 1 是取索引
    int nIndex = nGong - 1;
    // 这里要把寄宫转成位置
    m_nJiGongPos = m_nGuaNum2Pos[nIndex];
}

bool CQimen::BaseRun(const QiParam& info, CalendarType type) {
    m_calType = type;
    CCalenderFactory::freeCalender(m_pCal);
    m_pCal = CCalenderFactory::creatInstance(m_calType);
    if (!m_pCal->checkFormat(info.datetime)) {
        return false;
    }
    m_datetime = info.datetime;
    return true;
}

// 获取六十甲子下标
int CQimen::getJiaziIndex(int nTianIndex, int nDiIndex) {
    int nRet = nDiIndex;
    for (int i = 0; i < 5; ++i) {
        if ((nRet % 10) == nTianIndex) {
            break;
        }
        nRet += 12;
    }
    return nRet;
}

// 查找内容所在的下标
int CQimen::getIndex(const int* pData, int nSize, int nValue) {
    if (!pData) {
        return -1;
        ;
    }

    int nRe = -1;
    for (int i = 0; i < nSize; ++i) {
        if (pData[i] == nValue) {
            nRe = i;
            break;
        }
    }
    return nRe;
}

// 获取天盘
const int* CQimen::getTianPan() const { return m_nTianPan; }

// 获取地盘
const int* CQimen::getDiPan() const { return m_nDiPan; }

// 获取九星
const int* CQimen::getJiuXing() const { return m_JiuXingRe; }

// 获取九星
const int* CQimen::getJiuXingPre() const { return m_JiuXingPre; }

// 获取八门
const int* CQimen::getBaMen() const { return m_nBamenRe; }

const int* CQimen::getBaMenPre() const { return m_nBamenPre; }

// 获取八神
const int* CQimen::getBaShen() const { return m_nBaShenRe; }

// 获取马星
int CQimen::getMaXing() const { return m_nMaXing; }
// 获取空亡 (size 需要>= 2)
const int* CQimen::getKong() const { return m_nKongWang; }
// 获取日历
const CCalenderBase* CQimen::getCalendar() const { return m_pCal; }

// 获取旬空信息
const int* CQimen::getXunKong() const { return m_nXunKong; }

// 获取值班人员
int CQimen::getDuty() const { return m_nZhiFuPos; }

// 获取寄宫
int CQimen::getJiGong() const { return m_nJiGongPos; }

// 获取三元
int CQimen::getYuan() const { return m_nYuan; }
// 获取当日节气
int CQimen::getJieQi() const { return m_nJieQi; }
// 获取局数
int CQimen::getJushu() const { return m_nJushu; }

bool CQimen::getIsYinDun() const { return m_isYinDun; }

void CQimen::BaseInit() {
    setJiuXingPre();
    setJiuXingTurn();
    setGongBaseNum();
    setBaMenPre();
    setBaMenTurn();
    setBaShenPre();
    setDizhi();

    prepare();
    // 默认寄宫寄 坤二宫
    setJiGong(2);
}

}  // namespace cppbox
