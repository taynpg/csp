// csp - 奇门遁甲排盘库（内部）
// 二十四节气的局数与阴阳遁表、五不遇时表。
#ifndef CSP_SRC_JU_PATTERN_HPP
#define CSP_SRC_JU_PATTERN_HPP

#include <array>

#include "csp/types.hpp"

namespace csp::detail {

/// 某个节气上、中、下三元的局数，以及该节气是否阳遁。
struct JuPattern {
    int upper{};
    int middle{};
    int lower{};
    bool yang{};
};

/// 下标为节气序号（0 = 冬至，1 = 小寒 ... 23 = 大雪）。
/// 原实现把三元局数打包成一个四位整数（如 1741 表示上 1 中 7 下 4，末位 1 表示阳遁），
/// 这里拆成显式字段，避免 /1000、%10 之类的魔数解包。
inline constexpr std::array<JuPattern, kSolarTermCount> kJuPatterns{{
    {1, 7, 4, true},     // 冬至
    {2, 8, 5, true},     // 小寒
    {3, 9, 6, true},     // 大寒
    {8, 5, 2, true},     // 立春
    {9, 6, 3, true},     // 雨水
    {1, 7, 4, true},     // 惊蛰
    {3, 9, 6, true},     // 春分
    {4, 1, 7, true},     // 清明
    {5, 2, 8, true},     // 谷雨
    {4, 1, 7, true},     // 立夏
    {5, 2, 8, true},     // 小满
    {6, 3, 9, true},     // 芒种
    {9, 3, 6, false},    // 夏至
    {8, 2, 5, false},    // 小暑
    {7, 1, 4, false},    // 大暑
    {2, 5, 8, false},    // 立秋
    {1, 4, 7, false},    // 处暑
    {9, 3, 6, false},    // 白露
    {7, 1, 4, false},    // 秋分
    {6, 9, 3, false},    // 寒露
    {5, 8, 2, false},    // 霜降
    {6, 9, 3, false},    // 立冬
    {5, 8, 2, false},    // 小雪
    {4, 7, 1, false},    // 大雪
}};

/// 取某节气在指定元上的局数
inline int ju_of(const JuPattern& pattern, Yuan yuan)
{
    switch (yuan) {
    case Yuan::Upper:
        return pattern.upper;
    case Yuan::Middle:
        return pattern.middle;
    default:
        return pattern.lower;
    }
}

/// 五不遇时：下标为日干，值为与该日干相克的时干支序号。
/// 例如甲日庚午时（6）、乙日辛巳时（17）。
inline constexpr std::array<int, kStemCount> kFiveAvoidHours{6, 17, 28, 39, 50, 1, 12, 33, 44, 55};

}   // namespace csp::detail

#endif   // CSP_SRC_JU_PATTERN_HPP
