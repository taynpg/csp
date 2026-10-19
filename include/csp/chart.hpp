// csp - 奇门遁甲排盘库
// 排盘结果的数据结构。
#ifndef CSP_CHART_HPP
#define CSP_CHART_HPP

#include <array>
#include <optional>
#include <tyme.h>

#include "csp/types.hpp"

namespace csp {

/// 一张盘的全部计算结果。
///
/// 除特别说明外，所有数组下标都是本程序内部定义的宫位（0-8，见 qimen.hpp 顶部示意图），
/// 数组元素的值则是该宫位上的符号编号（九星 0-8、八门 0-8、八神 0-8、天干 0-9、地支 0-11）。
struct Chart {
    int ju{};                  ///< 局数（正数，阴阳见 yin_dun）
    Yuan yuan{Yuan::Manual};   ///< 三元
    bool yin_dun{};            ///< 是否阴遁
    bool wu_bu_yu{};           ///< 是否五不遇时

    std::array<int, kPalaceCount> stars{};              ///< 九星：宫位 -> 星
    std::array<int, kPalaceCount> stars_origin{};       ///< 九星的原始位置：宫位 -> 星
    std::array<int, kOuterPalaceCount> stars_cycle{};   ///< 九星的旋转顺序（8 个）

    std::array<int, kPalaceCount> gates{};              ///< 八门：宫位 -> 门
    std::array<int, kPalaceCount> gates_origin{};       ///< 八门的原始位置
    std::array<int, kOuterPalaceCount> gates_cycle{};   ///< 八门的旋转顺序（8 个）

    std::array<int, kPalaceCount> deities{};          ///< 八神：宫位 -> 神
    std::array<int, kPalaceCount> deities_origin{};   ///< 八神的原始位置

    std::array<int, kPalaceCount> heaven_stem{};   ///< 天盘：宫位 -> 天干
    std::array<int, kPalaceCount> earth_stem{};    ///< 地盘：宫位 -> 天干
    std::array<int, kPalaceCount> element{};       ///< 五行：1 金 2 水 3 木 4 火 5 土
    std::array<int, kPalaceCount> hidden_stem{};   ///< 暗干：宫位 -> 天干

    std::array<int, kPalaceCount> palace_to_trigram{};   ///< 内部位置 -> 八卦序号
    std::array<int, kPalaceCount> trigram_to_palace{};   ///< 八卦序号 -> 内部位置

    int lodging_palace{};                              ///< 天禽星的寄宫（内部位置）
    int horse_palace{-1};                              ///< 马星所在宫位，-1 表示未计算
    std::array<int, 2> void_palace{{-1, -1}};          ///< 时柱旬空所在的宫位
    std::array<std::array<int, 2>, 4> void_branch{};   ///< 年、月、日、时柱的旬空地支
    int duty_palace{-1};                               ///< 值符/值使所在的宫位
    int solar_term{};                                  ///< 当日节气序号（0 = 冬至）
    int moon_general{};                                ///< 月将（仅阴盘使用）

    std::array<int, kBranchCount> branch_palace{};   ///< 地支 -> 宫位
    std::array<int, kBranchCount> trine_leader{};    ///< 地支 -> 所属三合局的长生支
    std::array<int, kBranchCount> horse_branch{};    ///< 三合局长生支 -> 驿马支（稀疏表）

    std::optional<tyme::SolarTime> time;   ///< 排盘时间
};

}   // namespace csp

#endif   // CSP_CHART_HPP
