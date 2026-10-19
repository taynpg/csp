// csp - 奇门遁甲排盘库
// 中文名称表（简体）。如需其它语种，替换本文件即可。
#ifndef CSP_ZH_HPP
#define CSP_ZH_HPP

#include <string>
#include <string_view>

#include "csp/types.hpp"

namespace csp::zh {

std::string_view numeral(int index);        ///< 数字：零 一 ... 十
std::string month(int index);               ///< 农历月份：正 二 ... 腊（负数表示闰月，带「（闰）」前缀）
std::string_view lunar_day(int index);      ///< 农历日：初一 ... 三十
std::string_view solar_term(int index);     ///< 节气：0 = 冬至
std::string_view heaven_stem(int index);    ///< 天干：甲 ... 癸
std::string_view earth_branch(int index);   ///< 地支：子 ... 亥
std::string_view sexagenary(int index);     ///< 六十甲子：甲子 ... 癸亥
std::string_view star(int index);           ///< 九星：天蓬 ... 天英
std::string_view gate(int index);           ///< 八门：休门 ... 景门
std::string_view deity(int index);          ///< 八神：值符 ... 九天
std::string_view trigram(int index);        ///< 八卦：坎 坤 震 巽 中 乾 兑 艮 离
std::string_view element(int index);        ///< 五行：1 金 2 水 3 木 4 火 5 土
std::string_view void_mark();               ///< 空亡标记
std::string_view horse_mark();              ///< 马星标记

std::string_view plate_name(QimenType type);   ///< 盘式名称
std::string_view yuan_name(Yuan yuan);         ///< 三元名称

}   // namespace csp::zh

#endif   // CSP_ZH_HPP
