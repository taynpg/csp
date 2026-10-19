#include "test_framework.hpp"

#include <memory>

#include "csp/calendar.hpp"
#include "csp/qimen.hpp"
#include "csp/qimen_variants.hpp"
#include "csp/zh.hpp"

using namespace csp;

namespace {

std::unique_ptr<Qimen> build(QimenType type, const DateTime& time, int ju = 0,
                             HiddenStemStyle style = HiddenStemStyle::ByOriginalPalace)
{
    auto qimen = Qimen::create(type);
    if (qimen) {
        qimen->generate(time, ju, style);
    }
    return qimen;
}

}   // namespace

CSP_TEST(zhirun_2026_10_19)
{
    const auto qimen = build(QimenType::ZhiRun, DateTime(2026, 10, 19, 8, 25, 0));
    CHECK(qimen != nullptr);
    if (!qimen) {
        return;
    }
    const Chart& chart = qimen->chart();

    CHECK_EQ(chart.ju, 5);
    CHECK_EQ(chart.yin_dun, true);
    CHECK(chart.yuan == Yuan::Upper);
    CHECK_EQ(zh::solar_term(chart.solar_term), "霜降");

    // 值符天冲、值使伤门
    CHECK_EQ(chart.duty_palace, 5);
    CHECK_EQ(zh::star(chart.stars_origin[chart.duty_palace]), "天冲");
    CHECK_EQ(zh::gate(chart.gates_origin[chart.duty_palace]), "伤门");

    // 时柱午未空，马星在艮八宫（位置 6）
    CHECK_EQ(chart.horse_palace, 6);
    CHECK_EQ(chart.void_palace[0], 3);
    CHECK_EQ(chart.void_palace[1], 2);

    CHECK_EQ(zh::star(chart.stars[7]), "天冲");
    CHECK_EQ(zh::gate(chart.gates[4]), "伤门");
    CHECK_EQ(zh::star(chart.stars[kCenterPalace]), "天禽");
}

CSP_TEST(chaibu_1986_05_29)
{
    const auto qimen = build(QimenType::ChaiBu, DateTime(1986, 5, 29, 12, 0, 0));
    CHECK(qimen != nullptr);
    if (!qimen) {
        return;
    }
    const Chart& chart = qimen->chart();

    CHECK_EQ(chart.ju, 2);
    CHECK_EQ(chart.yin_dun, false);
    CHECK(chart.yuan == Yuan::Middle);
    CHECK_EQ(zh::solar_term(chart.solar_term), "小满");
    CHECK_EQ(chart.duty_palace, 1);
    CHECK_EQ(zh::star(chart.stars_origin[chart.duty_palace]), "天柱");
    CHECK_EQ(zh::gate(chart.gates_origin[chart.duty_palace]), "惊门");
}

// 手动定局时，局数的符号决定阴阳遁（阴盘曾忽略符号，此处作为回归测试）
CSP_TEST(manual_ju_sign_sets_dun_for_all_types)
{
    const DateTime time(2026, 10, 19, 8, 25, 0);
    const QimenType types[4] = {QimenType::ZhiRun, QimenType::YinPan, QimenType::ChaiBu, QimenType::MaoShan};

    for (const auto type : types) {
        const auto yin = build(type, time, -5);
        CHECK(yin != nullptr);
        if (yin) {
            CHECK_EQ(yin->chart().ju, 5);
            CHECK_EQ(yin->chart().yin_dun, true);
            CHECK(yin->chart().yuan == Yuan::Manual);
        }

        const auto yang = build(type, time, 5);
        CHECK(yang != nullptr);
        if (yang) {
            CHECK_EQ(yang->chart().ju, 5);
            CHECK_EQ(yang->chart().yin_dun, false);
        }
    }
}

CSP_TEST(automatic_ju_differs_between_styles)
{
    // 阴盘的定局方式与其它盘式不同，故局数/三元也不同
    const DateTime time(2026, 10, 19, 8, 25, 0);
    const auto yin_pan = build(QimenType::YinPan, time);
    CHECK(yin_pan != nullptr);
    if (yin_pan) {
        CHECK(yin_pan->chart().ju >= 1 && yin_pan->chart().ju <= 9);
        CHECK(yin_pan->chart().yuan == Yuan::Auto);
    }
}

CSP_TEST(hidden_stem_styles_differ)
{
    const DateTime time(2026, 10, 19, 8, 25, 0);
    const auto by_palace = build(QimenType::ZhiRun, time, 0, HiddenStemStyle::ByOriginalPalace);
    const auto by_gate = build(QimenType::ZhiRun, time, 0, HiddenStemStyle::ByDutyGate);
    CHECK(by_palace != nullptr && by_gate != nullptr);
    if (by_palace && by_gate) {
        CHECK(by_palace->chart().hidden_stem != by_gate->chart().hidden_stem);
    }
}

// 同一实例反复排盘，结果必须稳定（年历不能累积脏数据）
CSP_TEST(regenerate_on_same_instance_is_stable)
{
    auto qimen = Qimen::create(QimenType::ZhiRun);
    CHECK(qimen != nullptr);
    if (!qimen) {
        return;
    }

    const DateTime time(2026, 10, 19, 8, 25, 0);
    CHECK(qimen->generate(time, 0));
    const Chart first = qimen->chart();

    CHECK(qimen->generate(time, 0));
    const Chart& second = qimen->chart();

    CHECK(first.stars == second.stars);
    CHECK(first.gates == second.gates);
    CHECK(first.deities == second.deities);
    CHECK(first.earth_stem == second.earth_stem);
    CHECK(first.heaven_stem == second.heaven_stem);
    CHECK_EQ(first.ju, second.ju);
}

CSP_TEST(reject_out_of_range_year)
{
    const auto too_early = build(QimenType::ZhiRun, DateTime(1, 1, 1, 0, 0, 0));
    CHECK(too_early != nullptr);
    if (too_early) {
        CHECK_EQ(too_early->chart().time.has_value(), false);
    }

    const auto too_late = build(QimenType::ZhiRun, DateTime(9999, 1, 1, 0, 0, 0));
    CHECK(too_late != nullptr);
    if (too_late) {
        CHECK_EQ(too_late->chart().time.has_value(), false);
    }
}

CSP_TEST(reject_illegal_datetime)
{
    const auto bad_day = build(QimenType::ZhiRun, DateTime(2026, 13, 45, 0, 0, 0));
    CHECK(bad_day != nullptr);
    if (bad_day) {
        CHECK_EQ(bad_day->chart().time.has_value(), false);
    }
}

// 暗干入值使门：八宫应当是九个天干中互不相同的八个。
// v1.5.5 在"旬首仪落中宫"时会把 -1 当成宫位下标（越界读写），排出重复的天干。
CSP_TEST(hidden_stem_never_repeats_by_gate_style)
{
    // 逐日扫描一整年：值使门落中宫的盘约占十分之一，必须扫足够多的盘才能覆盖到
    int checked = 0;
    int center_duty = 0;
    for (int month = 1; month <= 12; ++month) {
        for (int day = 1; day <= 28; ++day) {
            for (int hour : {1, 13}) {
                for (QimenType type : {QimenType::ZhiRun, QimenType::YinPan, QimenType::ChaiBu, QimenType::MaoShan}) {
                    const auto qimen =
                        build(type, DateTime(2026, month, day, hour, 0, 0), 0, HiddenStemStyle::ByDutyGate);
                    if (!qimen || !qimen->chart().time.has_value()) {
                        continue;
                    }
                    const Chart& chart = qimen->chart();
                    if (chart.duty_palace == kCenterPalace) {
                        ++center_duty;
                    }
                    bool seen[10] = {};
                    for (int palace = 0; palace < kOuterPalaceCount; ++palace) {
                        const int stem = chart.hidden_stem[static_cast<std::size_t>(palace)];
                        CHECK(stem >= 1 && stem <= 9);
                        CHECK_EQ(seen[stem], false);
                        seen[stem] = true;
                    }
                    ++checked;
                }
            }
        }
    }
    CHECK(checked > 2000);
    CHECK(center_duty > 100);   // 确保覆盖到"值使门落中宫"这条路径
}

// 茅山法要取"上月同一天"的节气；月末日期（如 12-31）不能构造出 11-31 这种非法日期
CSP_TEST(maoshan_handles_month_end)
{
    const DateTime cases[] = {
        DateTime(2100, 12, 31, 23, 0, 0), DateTime(1999, 12, 31, 23, 30, 0), DateTime(2024, 3, 31, 12, 0, 0),
        DateTime(2026, 5, 31, 0, 0, 0),   DateTime(2023, 7, 31, 8, 0, 0),    DateTime(9998, 12, 31, 23, 59, 59),
    };
    for (const auto& time : cases) {
        const auto qimen = build(QimenType::MaoShan, time);
        CHECK(qimen != nullptr);
        if (qimen) {
            CHECK_EQ(qimen->chart().time.has_value(), true);
            CHECK(qimen->chart().ju >= 1 && qimen->chart().ju <= 9);
        }
    }
}

// 旬首仪落中宫时，值使门要按寄宫（坤二）取，再以它落宫起时干。
// v1.5.5 这里取不到门（返回 -1），随后把 -1 当宫位下标做越界读写，暗干出现重复天干。
CSP_TEST(hidden_stem_when_duty_stem_in_center_palace)
{
    const auto qimen = build(QimenType::ZhiRun, DateTime(2026, 10, 10, 12, 0, 0), 0, HiddenStemStyle::ByDutyGate);
    CHECK(qimen != nullptr);
    if (!qimen) {
        return;
    }
    const Chart& chart = qimen->chart();
    CHECK_EQ(chart.duty_palace, kCenterPalace);

    const char* expected[] = {"壬", "辛", "乙", "己", "丁", "丙", "庚", "戊"};
    for (int palace = 0; palace < kOuterPalaceCount; ++palace) {
        CHECK_EQ(zh::heaven_stem(chart.hidden_stem[static_cast<std::size_t>(palace)]), expected[palace]);
    }
}
