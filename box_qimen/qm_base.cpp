#include "qm_base.h"

#include <cstdio>

#include "qm_v1.h"
#include "qm_v2.h"
#include "qm_v3.h"

namespace cppbox {

// 获取实例
CQimen* CQimenFactory::createInstance(QIMEN_STYLE type)
{
    CQimen* pQimen = nullptr;
    switch (type) {
        case SHIJIA_ZHUANPAN_CHAOJIE_ZHIRUN: {   // 时家超接置润法
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
void CQimenFactory::freeInstance(CQimen* pQimen)
{
    delete pQimen;
}

CQimen::CQimen() : pcal_(nullptr), cal_type_(CALENDAR_V1)
{
    sanhe_[0] = 8;
    sanhe_[1] = 5;
    sanhe_[2] = 2;
    sanhe_[3] = 2;
    sanhe_[4] = 8;
    sanhe_[5] = 5;
    sanhe_[6] = 2;
    sanhe_[7] = 11;
    sanhe_[8] = 8;
    sanhe_[9] = 5;
    sanhe_[10] = 2;
    sanhe_[11] = 11;

    dzc_[8] = 2;
    dzc_[5] = 11;
    dzc_[2] = 8;
    dzc_[2] = 8;
    dzc_[11] = 5;
}

CQimen::~CQimen()
{
    CCalenderFactory::free(pcal_);
}

// 设置十二地支位置对应
void CQimen::setDizhi()
{
    zhi_[0] = 7;
    zhi_[1] = 6;
    zhi_[2] = 6;
    zhi_[3] = 5;
    zhi_[4] = 4;
    zhi_[5] = 4;
    zhi_[6] = 3;
    zhi_[7] = 2;
    zhi_[8] = 2;
    zhi_[9] = 1;
    zhi_[10] = 0;
    zhi_[11] = 0;
}

void CQimen::set_jx_pre()
{
    jx_pre_[0] = 5;
    jx_pre_[1] = 6;
    jx_pre_[2] = 1;
    jx_pre_[3] = 8;
    jx_pre_[4] = 3;
    jx_pre_[5] = 2;
    jx_pre_[6] = 7;
    jx_pre_[7] = 0;
    jx_pre_[8] = 4;
}

// 设置各位置的五行(金水木火土 数字代表 12345)
void CQimen::set_wx()
{
    wx_[0] = 1;
    wx_[1] = 1;
    wx_[2] = 5;
    wx_[3] = 4;
    wx_[4] = 3;
    wx_[5] = 3;
    wx_[6] = 5;
    wx_[7] = 2;
    wx_[8] = 5;
}

// 设置九星的旋转位置[相对顺序]
void CQimen::set_jx_turn()
{
    jx_turn_[0] = 0;
    jx_turn_[1] = 7;
    jx_turn_[2] = 2;
    jx_turn_[3] = 3;
    jx_turn_[4] = 8;
    jx_turn_[5] = 1;
    jx_turn_[6] = 6;
    jx_turn_[7] = 5;
}

// 设置八门的初始位置
void CQimen::set_bm_pre()
{
    bm_pre_[0] = 5;
    bm_pre_[1] = 6;
    bm_pre_[2] = 1;
    bm_pre_[3] = 8;
    bm_pre_[4] = 3;
    bm_pre_[5] = 2;
    bm_pre_[6] = 7;
    bm_pre_[7] = 0;
    bm_pre_[8] = 4;
}

// 设置八门的旋转位置
void CQimen::set_bm_turn()
{
    bm_turn_[0] = 0;
    bm_turn_[1] = 7;
    bm_turn_[2] = 2;
    bm_turn_[3] = 3;
    bm_turn_[4] = 8;
    bm_turn_[5] = 1;
    bm_turn_[6] = 6;
    bm_turn_[7] = 5;
}

// 设置八神的初始位置
void CQimen::set_bs_pre()
{
    bs_pre_[0] = 5;
    bs_pre_[1] = 6;
    bs_pre_[2] = 1;
    bs_pre_[3] = 8;
    bs_pre_[4] = 3;
    bs_pre_[5] = 2;
    bs_pre_[6] = 7;
    bs_pre_[7] = 0;
    bs_pre_[8] = 4;
}

// 设置九宫的数字对应
void CQimen::set_gong_base()
{
    // 定义位置从 0 开始分别对应卦序
    pos2gua_[0] = 5;
    pos2gua_[1] = 6;
    pos2gua_[2] = 1;
    pos2gua_[3] = 8;
    pos2gua_[4] = 3;
    pos2gua_[5] = 2;
    pos2gua_[6] = 7;
    pos2gua_[7] = 0;
    pos2gua_[8] = 4;
}

// 设置初始内容之后，执行此函数，对初始内容整理并生成所需的信息。
void CQimen::prepare()
{
    // gua2pos_ 就是后天八卦的顺序数和定义位置的对照。
    for (int i = 0; i < 9; ++i) {
        gua2pos_[pos2gua_[i]] = i;
    }
}

// 设置寄宫
void CQimen::set_jg(int nGong)
{
    // 这里减 1 是取索引
    int nIndex = nGong - 1;
    // 这里要把寄宫转成位置
    jg_ = gua2pos_[nIndex];
}

bool CQimen::base_run(const QiParam& info, CalendarType type)
{
    cal_type_ = type;
    CCalenderFactory::free(pcal_);
    pcal_ = CCalenderFactory::creat_instance(cal_type_);
    if (!pcal_->check_format(info.datetime_)) {
        std::snprintf(err_, sizeof(err_), "%s", "日期不在支持的范围内。");
        return false;
    }
    datetime_ = info.datetime_;
    return true;
}

const char* CQimen::getLastError() const
{
    return err_;
}

// 获取六十甲子下标
int CQimen::get_jz(int nTianIndex, int nDiIndex)
{
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
int CQimen::get_index(const int* pData, int nSize, int nValue)
{
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
const int* CQimen::get_tp() const
{
    return t_;
}

// 获取地盘
const int* CQimen::get_dp() const
{
    return d_;
}

// 获取九星
const int* CQimen::get_jx() const
{
    return jx_re_;
}

// 获取九星
const int* CQimen::get_jx_pre() const
{
    return jx_pre_;
}

// 获取八门
const int* CQimen::get_bm() const
{
    return bm_re_;
}

const int* CQimen::get_bm_pre() const
{
    return bm_pre_;
}

// 获取八神
const int* CQimen::get_bs() const
{
    return bs_re_;
}

// 获取马星
int CQimen::get_mx() const
{
    return mx_;
}
// 获取空亡 (size 需要>= 2)
const int* CQimen::get_kw() const
{
    return kw_;
}
// 获取日历
const CCalenderBase* CQimen::get_cal() const
{
    return pcal_;
}

// 获取旬空信息
const int* CQimen::get_xk() const
{
    return xk_;
}

// 获取值班人员
int CQimen::get_duty() const
{
    return zf_;
}

// 获取寄宫
int CQimen::get_jg() const
{
    return jg_;
}

// 获取三元
int CQimen::get_yuan() const
{
    return yuan_;
}
// 获取当日节气
int CQimen::get_jq() const
{
    return jq_;
}
// 获取局数
int CQimen::get_js() const
{
    return jushu_;
}

bool CQimen::is_yin() const
{
    return is_yin_;
}

// 获取五行
const int* CQimen::get_wx() const
{
    return wx_;
}

void CQimen::base_init()
{
    set_wx();
    set_jx_pre();
    set_jx_turn();
    set_gong_base();
    set_bm_pre();
    set_bm_turn();
    set_bs_pre();
    setDizhi();

    prepare();
    // 默认寄宫寄 坤二宫
    set_jg(2);
}

// 获取卦序号
const int* CQimen::get_guaxu() const
{
    return pos2gua_;
}

// 获取地支与位置的对应关系
const int* CQimen::get_zhi_pos() const
{
    return zhi_;
}

bool CQimen::is_wby() const
{
    return wbys_;
}

}   // namespace cppbox
