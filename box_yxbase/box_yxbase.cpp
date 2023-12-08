#include "box_yxbase.h"

//  0     1     2     3     4     5     6     7     8     9     10    11
// "子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"
//
// 甲亥乙午丙戊寅
// 庚巳辛子及壬申
// 丁己在酉癸卯寻

const int gInt_ZhangSheng[] = {11, 6, 2, 9, 2, 9, 5, 0, 8, 3};

namespace cppbox {

CYXBase::CYXBase() = default;

CYXBase::~CYXBase() = default;

const int* CYXBase::ZhangShengJue() { return gInt_ZhangSheng; }
}  // namespace cppbox
