// csp - 奇门遁甲排盘库
// 基础类型与常量定义。
#ifndef CSP_TYPES_HPP
#define CSP_TYPES_HPP

#include <array>
#include <cstddef>

namespace csp {

/// 九宫数量（本程序内部定义的位置，见 qimen.hpp 顶部示意图）
inline constexpr int kPalaceCount = 9;
/// 中五宫在内部定义位置中的下标
inline constexpr int kCenterPalace = 8;
/// 中宫之外的八宫数量
inline constexpr int kOuterPalaceCount = 8;
/// 八卦序号数量（0 坎 1 坤 2 震 3 巽 4 中 5 乾 6 兑 7 艮 8 离）
inline constexpr int kTrigramCount = 9;
/// 十二地支数量
inline constexpr int kBranchCount = 12;
/// 十天干数量
inline constexpr int kStemCount = 10;
/// 六十甲子数量
inline constexpr int kSexagenaryCount = 60;
/// 二十四节气数量（0 = 冬至）
inline constexpr int kSolarTermCount = 24;
/// 天禽星的编号
inline constexpr int kStarTianQin = 4;

/// 支持的盘式
enum class QimenType : int {
    ZhiRun = 1,    ///< 时家转盘，超接置闰法
    YinPan = 2,    ///< 时家转盘，阴盘
    ChaiBu = 3,    ///< 时家转盘，拆补法
    MaoShan = 4,   ///< 时家转盘，茅山法
};

/// 三元
enum class Yuan : int {
    Auto = -1,    ///< 自动定局（由盘式自行推演，无三元概念）
    Manual = 0,   ///< 手动指定局数
    Lower = 1,    ///< 下元
    Middle = 2,   ///< 中元
    Upper = 3,    ///< 上元
};

/// 暗干排法
enum class HiddenStemStyle : int {
    ByOriginalPalace = 0,   ///< 门原宫位地盘
    ByDutyGate = 1,         ///< 入值使门
};

/// 公历日期时间
struct DateTime {
    DateTime() = default;
    DateTime(int y, int m, int d, int h, int mi, int s) : year(y), mon(m), day(d), hour(h), min(mi), sec(s)
    {
    }

    int year{};
    int mon{};
    int day{};
    int hour{};
    int min{};
    int sec{};
    int msec{};
};

/// 四柱：各柱六十甲子的下标（0-59）
struct Pillars {
    int year{};
    int month{};
    int day{};
    int hour{};
};

/// 在数组的前 count 个元素中查找值的下标，找不到返回 -1
template <typename T, std::size_t M> int index_of(const std::array<T, M>& values, const T& value, std::size_t count = M)
{
    if (count > M) {
        count = M;
    }
    for (std::size_t i = 0; i < count; ++i) {
        if (values[i] == value) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

}   // namespace csp

#endif   // CSP_TYPES_HPP
