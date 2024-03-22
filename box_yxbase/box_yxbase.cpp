#include "box_yxbase.h"

//  0     1     2     3     4     5     6     7     8     9     10    11
// "子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"
//
// 甲亥乙午丙戊寅
// 庚巳辛子及壬申
// 丁己在酉癸卯寻
#include <map>

std::map<int, cppbox::WUXING_STATE> sk_{};

const int gInt_ZhangSheng[] = {11, 6, 2, 9, 2, 9, 5, 0, 8, 3};

namespace cppbox {

CYXBase::CYXBase()
{
    sk_[51] = WUXING_STATE::SHENG;
    sk_[52] = WUXING_STATE::KE;
    sk_[53] = WUXING_STATE::BEI_KE;
    sk_[54] = WUXING_STATE::BEI_SHENG;
    sk_[55] = WUXING_STATE::TONG;

    sk_[15] = WUXING_STATE::BEI_SHENG;
    sk_[14] = WUXING_STATE::BEI_KE;
    sk_[13] = WUXING_STATE::KE;
    sk_[12] = WUXING_STATE::SHENG;
    sk_[11] = WUXING_STATE::TONG;

    sk_[21] = WUXING_STATE::BEI_SHENG;
    sk_[22] = WUXING_STATE::TONG;
    sk_[23] = WUXING_STATE::SHENG;
    sk_[24] = WUXING_STATE::KE;
    sk_[25] = WUXING_STATE::BEI_KE;

    sk_[31] = WUXING_STATE::BEI_KE;
    sk_[32] = WUXING_STATE::BEI_SHENG;
    sk_[33] = WUXING_STATE::TONG;
    sk_[34] = WUXING_STATE::SHENG;
    sk_[35] = WUXING_STATE::KE;

    sk_[41] = WUXING_STATE::KE;
    sk_[42] = WUXING_STATE::BEI_KE;
    sk_[43] = WUXING_STATE::BEI_SHENG;
    sk_[44] = WUXING_STATE::TONG;
    sk_[45] = WUXING_STATE::SHENG;
}

CYXBase::~CYXBase() = default;

const int* CYXBase::ZhangShengJue() { return gInt_ZhangSheng; }

// 金水木火土 数字代表 12345
WUXING_STATE CYXBase::JudgeState(int me, int other)
{
    if (me < 0 || me > 5) {
        return WUXING_STATE::NUM_ERROR;
    }
    if (other < 0 || other > 5) {
        return WUXING_STATE::NUM_ERROR;
    }
    return sk_[me * 10 + other];
}

}   // namespace cppbox
