// csp - 奇门遁甲排盘库
// 各派盘式的实现。所有盘式共用 QimenV1 的排盘步骤，仅「定局」不同。
#ifndef CSP_QIMEN_VARIANTS_HPP
#define CSP_QIMEN_VARIANTS_HPP

#include <utility>
#include <vector>

#include "csp/qimen.hpp"

namespace csp {

/// 时家转盘，超接置闰法。
///
/// 一旬十个时辰由一个值符看管（九星之一）：
///   1. 地盘按超接法定遁；
///   2. 值符即当前时干旬头甲所遁之宫上的原始星；
///   3. 值使与值符同理；
///   4. 转动星盘：值符随时干，即值符放在时干的位置；
///   5. 转动门盘：值使随时支，即值使放在从旬头按阳顺阴逆排列的位置。
class QimenV1 : public Qimen
{
public:
    QimenV1() = default;

protected:
    bool compute_ju(const DateTime& dt) override;

    void arrange_earth_stems() override;
    void resolve_duty() override;
    void arrange_stars() override;
    void arrange_gates() override;
    void arrange_deities() override;
    void arrange_heaven_stems() override;
    void resolve_void_and_horse() override;
    void arrange_hidden_stems() override;

    /// 单日信息：某一天的节气与日干支
    struct DayInfo {
        DateTime date{};
        int solar_term{};
        int sexagenary{};
    };

    /// 依次推演一年的日历
    bool build_almanac();

    /// 在推演出的日历中查找当天的信息
    const DayInfo* find_day(const DateTime& dt) const;

    /// 取某时刻所在的节气与当日日干支
    void month_term(const DateTime& dt, DateTime& term_time, int& sexagenary) const;

    /// 追加一段日期信息
    /// @param cursor       游标日期，逐日推进
    /// @param solar_term   当前节气序号，遇节推进
    /// @param sexagenary   当前日干支
    /// @param stop_term    遇到该节气即停止
    /// @param days         最多追加的天数
    /// @param advance_term 是否换节
    void append_days(DateTime& cursor, int& solar_term, int& sexagenary, int stop_term, int days,
                     bool advance_term = true);

    /// 某个六十甲子的两个旬空地支
    static std::pair<int, int> void_branches(int sexagenary);

    /// 暗干：入值使门
    void hidden_stem_by_gate();

    /// 暗干：门原宫位地盘
    void hidden_stem_by_palace();

private:
    std::vector<DayInfo> almanac_{};
};

/// 时家转盘，阴盘（局数 = 年支数 + 农历月 + 农历日 + 时支数，取九余）。
class QimenV2 : public QimenV1
{
public:
    QimenV2() = default;

protected:
    bool compute_ju(const DateTime& dt) override;
    bool resolve_ju(const DateTime& dt) override;

private:
    void update_moon_general();
};

/// 时家转盘，拆补法。
class QimenV3 : public QimenV1
{
public:
    QimenV3() = default;

protected:
    bool compute_ju(const DateTime& dt) override;
};

/// 时家转盘，茅山法（按交节后的天数定三元）。
class QimenV4 : public QimenV1
{
public:
    QimenV4() = default;

protected:
    bool compute_ju(const DateTime& dt) override;

private:
    /// 按距交节的时间定三元
    static Yuan yuan_of(int seconds_since_term);
};

}   // namespace csp

#endif   // CSP_QIMEN_VARIANTS_HPP
