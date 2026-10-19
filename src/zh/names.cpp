#include "csp/zh.hpp"

#include <array>
#include <string>

namespace csp::zh {

namespace {

constexpr std::array<std::string_view, 11> kNumeral{"零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"};

constexpr std::array<std::string_view, 12> kMonth{"正", "二", "三", "四", "五", "六",
                                                  "七", "八", "九", "十", "冬", "腊"};

/// 0 = 冬至
constexpr std::array<std::string_view, kSolarTermCount> kSolarTerm{
    "冬至", "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏", "小满", "芒种",
    "夏至", "小暑", "大暑", "立秋", "处暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪",
};

constexpr std::array<std::string_view, 30> kLunarDay{
    "初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十",
    "十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八", "十九", "二十",
    "廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十",
};

constexpr std::array<std::string_view, kStemCount> kHeavenStem{"甲", "乙", "丙", "丁", "戊",
                                                               "己", "庚", "辛", "壬", "癸"};

constexpr std::array<std::string_view, kBranchCount> kEarthBranch{"子", "丑", "寅", "卯", "辰", "巳",
                                                                  "午", "未", "申", "酉", "戌", "亥"};

constexpr std::array<std::string_view, kSexagenaryCount> kSexagenary{
    "甲子", "乙丑", "丙寅", "丁卯", "戊辰", "己巳", "庚午", "辛未", "壬申", "癸酉", "甲戌", "乙亥",
    "丙子", "丁丑", "戊寅", "己卯", "庚辰", "辛巳", "壬午", "癸未", "甲申", "乙酉", "丙戌", "丁亥",
    "戊子", "己丑", "庚寅", "辛卯", "壬辰", "癸巳", "甲午", "乙未", "丙申", "丁酉", "戊戌", "己亥",
    "庚子", "辛丑", "壬寅", "癸卯", "甲辰", "乙巳", "丙午", "丁未", "戊申", "己酉", "庚戌", "辛亥",
    "壬子", "癸丑", "甲寅", "乙卯", "丙辰", "丁巳", "戊午", "己未", "庚申", "辛酉", "壬戌", "癸亥",
};

constexpr std::array<std::string_view, kPalaceCount> kStar{"天蓬", "天芮", "天冲", "天辅", "天禽",
                                                           "天心", "天柱", "天任", "天英"};

constexpr std::array<std::string_view, kPalaceCount> kGate{"休门", "死门", "伤门", "杜门", "死门",
                                                           "开门", "惊门", "生门", "景门"};

constexpr std::array<std::string_view, kPalaceCount> kDeity{"值符", "腾蛇", "太阴", "六合", "腾蛇",
                                                            "白虎", "玄武", "九地", "九天"};

constexpr std::array<std::string_view, kTrigramCount> kTrigram{"坎", "坤", "震", "巽", "中", "乾", "兑", "艮", "离"};

/// 0 表示无五行
constexpr std::array<std::string_view, 6> kElement{"", "金", "水", "木", "火", "土"};

template <typename T, std::size_t N> std::string_view at(const std::array<T, N>& table, int index)
{
    if (index < 0 || static_cast<std::size_t>(index) >= N) {
        return {};
    }
    return table[static_cast<std::size_t>(index)];
}

}   // namespace

std::string_view numeral(int index)
{
    return at(kNumeral, index);
}

std::string month(int index)
{
    // 负数表示闰月，例如闰五月传入 -6
    int real = index;
    if (real < 0) {
        real = -real - 2;
    }
    const auto name = at(kMonth, real);
    if (name.empty()) {
        return {};
    }
    return index < 0 ? "（闰）" + std::string(name) : std::string(name);
}

std::string_view lunar_day(int index)
{
    return at(kLunarDay, index);
}

std::string_view solar_term(int index)
{
    return at(kSolarTerm, index);
}

std::string_view heaven_stem(int index)
{
    return at(kHeavenStem, index);
}

std::string_view earth_branch(int index)
{
    return at(kEarthBranch, index);
}

std::string_view sexagenary(int index)
{
    return at(kSexagenary, index);
}

std::string_view star(int index)
{
    return at(kStar, index);
}

std::string_view gate(int index)
{
    return at(kGate, index);
}

std::string_view deity(int index)
{
    return at(kDeity, index);
}

std::string_view trigram(int index)
{
    return at(kTrigram, index);
}

std::string_view element(int index)
{
    return at(kElement, index);
}

std::string_view void_mark()
{
    return "空";
}

std::string_view horse_mark()
{
    return "马";
}

std::string_view plate_name(QimenType type)
{
    switch (type) {
    case QimenType::ZhiRun:
        return "时家转盘超接置润法";
    case QimenType::YinPan:
        return "时家阴盘";
    case QimenType::ChaiBu:
        return "时家转盘拆补法";
    case QimenType::MaoShan:
        return "时家转盘茅山法";
    }
    return {};
}

std::string_view yuan_name(Yuan yuan)
{
    switch (yuan) {
    case Yuan::Auto:
        return "自动定局";
    case Yuan::Manual:
        return "手动定局";
    case Yuan::Lower:
        return "下元";
    case Yuan::Middle:
        return "中元";
    case Yuan::Upper:
        return "上元";
    }
    return {};
}

}   // namespace csp::zh
