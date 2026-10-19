#include "csp/qimen_variants.hpp"

#include <cstdlib>
#include <utility>

#include "csp/calendar.hpp"
#include "ju_pattern.hpp"

namespace csp {

// ---------------------------------------------------------------- 定局

bool QimenV1::compute_ju(const DateTime& dt)
{
    if (!build_almanac()) {
        return false;
    }
    const DayInfo* info = find_day(dt);
    if (info == nullptr) {
        return false;
    }

    chart_.solar_term = info->solar_term;
    const auto& pattern = detail::kJuPatterns[info->solar_term];
    chart_.yin_dun = !pattern.yang;

    // 每 15 天一元，5 天一个符头
    switch ((info->sexagenary % 15) / 5) {
    case 0:
        chart_.yuan = Yuan::Upper;
        break;
    case 1:
        chart_.yuan = Yuan::Middle;
        break;
    default:
        chart_.yuan = Yuan::Lower;
        break;
    }
    chart_.ju = detail::ju_of(pattern, chart_.yuan);
    return true;
}

// ---------------------------------------------------------------- 地盘

void QimenV1::arrange_earth_stems()
{
    // 所谓几局，就是甲子戊居于第几宫
    int trigram = cal::wrap(kTrigramCount, chart_.ju - 1);
    chart_.earth_stem[chart_.trigram_to_palace[trigram]] = 4;   // 戊

    const int step = chart_.yin_dun ? -1 : 1;
    for (int i = 0; i < 5; ++i) {   // 己 庚 辛 壬 癸
        trigram = cal::wrap(kTrigramCount, trigram + step);
        chart_.earth_stem[chart_.trigram_to_palace[trigram]] = 5 + i;
    }
    for (int i = 0; i < 3; ++i) {   // 丁 丙 乙
        trigram = cal::wrap(kTrigramCount, trigram + step);
        chart_.earth_stem[chart_.trigram_to_palace[trigram]] = 3 - i;
    }

    const auto& p = pillars();
    chart_.wu_bu_yu = (detail::kFiveAvoidHours[p.day % kStemCount] == p.hour);
}

// ---------------------------------------------------------------- 值符值使

void QimenV1::resolve_duty()
{
    // 旬头所遁的六仪：甲子戊、甲戌己、甲申庚、甲午辛、甲辰壬、甲寅癸
    const int stem = (pillars().hour / 10) + 4;
    chart_.duty_palace = index_of(chart_.earth_stem, stem);
}

// ---------------------------------------------------------------- 九星

void QimenV1::arrange_stars()
{
    const auto& p = pillars();
    // 时干为甲时为伏吟，星门皆归原宫
    if (p.hour % kStemCount == 0) {
        chart_.stars = chart_.stars_origin;
        chart_.gates = chart_.gates_origin;
        return;
    }

    // 时干在地盘的位置
    int palace = index_of(chart_.earth_stem, p.hour % kStemCount);
    if (palace == kCenterPalace) {
        palace = chart_.lodging_palace;
    }

    int star = chart_.stars_origin[chart_.duty_palace];
    if (star == kStarTianQin) {   // 值符为天禽时，用寄宫之星
        star = chart_.stars_origin[chart_.lodging_palace];
    }
    chart_.stars[palace] = star;

    int cycle = index_of(chart_.stars_cycle, star);
    for (int i = 0; i < kOuterPalaceCount; ++i) {
        palace = cal::wrap(kOuterPalaceCount, --palace);
        cycle = cal::wrap(kOuterPalaceCount, ++cycle);
        chart_.stars[palace] = chart_.stars_cycle[cycle];
    }
    chart_.stars[kCenterPalace] = kStarTianQin;
}

// ---------------------------------------------------------------- 八门

void QimenV1::arrange_gates()
{
    const int jumps = pillars().hour % kStemCount;   // 距符头几跳
    const int step = chart_.yin_dun ? -1 : 1;

    // 值使落在中宫时，从寄宫起转
    int start = chart_.duty_palace;
    if (start == kCenterPalace) {
        start = chart_.lodging_palace;
    }

    int trigram = chart_.palace_to_trigram[chart_.duty_palace];
    for (int i = 0; i < jumps; ++i) {
        trigram = cal::wrap(kTrigramCount, trigram + step);
    }
    int palace = chart_.trigram_to_palace[trigram];
    if (palace == kCenterPalace) {
        palace = chart_.lodging_palace;
    }

    int cycle = index_of(chart_.gates_cycle, chart_.gates_origin[start]);
    for (int i = 0; i < kOuterPalaceCount; ++i, --palace, ++cycle) {
        chart_.gates[cal::wrap(kOuterPalaceCount, palace)] = chart_.gates_cycle[cal::wrap(kOuterPalaceCount, cycle)];
    }
}

// ---------------------------------------------------------------- 八神

void QimenV1::arrange_deities()
{
    int star = chart_.stars_origin[chart_.duty_palace];
    if (star == kStarTianQin) {
        star = chart_.stars_origin[chart_.lodging_palace];
    }
    // 值符被转到哪个宫，就从哪里开始排
    int palace = index_of(chart_.stars, star);
    const int step = chart_.yin_dun ? 1 : -1;
    for (int i = 0; i < kPalaceCount; ++i, palace += step) {
        if (i == 4) {   // 中宫不排神
            ++i;
        }
        chart_.deities[cal::wrap(kOuterPalaceCount, palace)] = i;
    }
}

// ---------------------------------------------------------------- 天盘

void QimenV1::arrange_heaven_stems()
{
    // 当前星所在宫位的天盘，就是该星原始宫位上的地盘
    for (int palace = 0; palace < kOuterPalaceCount; ++palace) {
        chart_.heaven_stem[palace] = chart_.earth_stem[chart_.trigram_to_palace[chart_.stars[palace]]];
    }
    chart_.heaven_stem[kCenterPalace] = chart_.earth_stem[kCenterPalace];
}

// ---------------------------------------------------------------- 旬空、马星

void QimenV1::resolve_void_and_horse()
{
    const auto& p = pillars();
    const int sexagenaries[4] = {p.year, p.month, p.day, p.hour};
    for (int i = 0; i < 4; ++i) {
        const auto range = void_branches(sexagenaries[i]);
        chart_.void_branch[i][0] = range.first;
        chart_.void_branch[i][1] = range.second;
    }

    // 时柱旬空所在宫位
    chart_.void_palace[0] = chart_.branch_palace[chart_.void_branch[3][0]];
    chart_.void_palace[1] = chart_.branch_palace[chart_.void_branch[3][1]];

    // 马星：时支 -> 三合局长生支 -> 驿马支 -> 宫位
    const int horse = chart_.horse_branch[chart_.trine_leader[p.hour % kBranchCount]];
    chart_.horse_palace = chart_.branch_palace[horse];
}

// ---------------------------------------------------------------- 暗干

void QimenV1::arrange_hidden_stems()
{
    if (hidden_stem_style_ == HiddenStemStyle::ByDutyGate) {
        hidden_stem_by_gate();
    } else {
        hidden_stem_by_palace();
    }
}

void QimenV1::hidden_stem_by_gate()
{
    int stem = pillars().hour % kStemCount;   // 时干

    // 值使门取旬首仪所落之宫的门；该宫为中宫时按寄宫（坤二）取，
    // 因为八门不入中宫，gates_origin[中宫] 只是占位值。
    const int duty_start = (chart_.duty_palace == kCenterPalace) ? chart_.lodging_palace : chart_.duty_palace;
    const int duty_gate = chart_.gates_origin[duty_start];
    const int gate_palace = index_of(chart_.gates, duty_gate);

    // 值使门所在宫的地盘若是时干，则从寄宫起排
    int start = kCenterPalace;
    if (gate_palace >= 0 && chart_.earth_stem[gate_palace] != stem) {
        start = gate_palace;
    }

    const int step = chart_.yin_dun ? -1 : 1;
    int trigram = chart_.palace_to_trigram[start];
    for (int i = 0; i < kPalaceCount; ++i) {
        chart_.hidden_stem[start] = stem;
        stem = cal::wrap(kStemCount, stem + 1);
        stem = (stem == 0 ? 1 : stem);
        trigram = cal::wrap(kTrigramCount, trigram + step);
        start = index_of(chart_.palace_to_trigram, trigram);
    }

    // 甲、乙、丙单独处理：甲不排暗干（记为癸），乙丙互换
    const int jia = index_of(chart_.hidden_stem, 0);
    if (jia >= 0) {
        chart_.hidden_stem[jia] = 9;
    }
    const int bing = index_of(chart_.hidden_stem, 3);
    const int yi = index_of(chart_.hidden_stem, 1);
    if (bing >= 0 && yi >= 0) {
        std::swap(chart_.hidden_stem[bing], chart_.hidden_stem[yi]);
    }
}

void QimenV1::hidden_stem_by_palace()
{
    for (int palace = 0; palace < kOuterPalaceCount; ++palace) {
        const int origin = index_of(chart_.gates_origin, chart_.gates[palace]);
        chart_.hidden_stem[palace] = (origin < 0) ? 0 : chart_.earth_stem[origin];
    }
}

// ---------------------------------------------------------------- 推演年历

void QimenV1::month_term(const DateTime& dt, DateTime& term_time, int& sexagenary) const
{
    const auto term = cal::terms_around(cal::to_solar(dt)).previous;
    term_time = cal::from_solar(term.time);
    sexagenary = cal::four_pillars_without_hour(term.time).day;
}

bool QimenV1::build_almanac()
{
    almanac_.clear();

    const int year = time().get_year();
    constexpr int kStartTerm = 23;   // 从大雪开始推演

    DateTime term_time{};
    int sexagenary = 0;

    // 以上一年 12 月 1 日所在的节气为起点，回退到符头（甲、己日）
    month_term(DateTime(year - 1, 12, 1, 12, 0, 0), term_time, sexagenary);
    int offset = sexagenary - (sexagenary / 15) * 15;

    int term = kStartTerm;
    int day = cal::wrap(kSexagenaryCount, sexagenary - offset);
    DateTime cursor = cal::add_seconds(term_time, -offset * 24 * 3600);

    append_days(cursor, term, day, -1, 15, std::abs(offset) < 9);
    append_days(cursor, term, day, 11, 300);   // 推到芒种

    month_term(DateTime(year, 6, 6, 12, 0, 0), term_time, sexagenary);
    offset = std::abs(cal::days_between(cursor, term_time));
    append_days(cursor, term, day, -1, 15, std::abs(offset) < 9);
    append_days(cursor, term, day, 23, 300);   // 推到大雪

    month_term(DateTime(year, 12, 1, 12, 0, 0), term_time, sexagenary);
    offset = std::abs(cal::days_between(cursor, term_time));
    append_days(cursor, term, day, -1, 15, std::abs(offset) < 9);
    append_days(cursor, term, day, -1, 60);

    return true;
}

const QimenV1::DayInfo* QimenV1::find_day(const DateTime& dt) const
{
    for (const auto& item : almanac_) {
        if (item.date.year == dt.year && item.date.mon == dt.mon && item.date.day == dt.day) {
            return &item;
        }
    }
    return nullptr;
}

void QimenV1::append_days(DateTime& cursor, int& solar_term, int& sexagenary, int stop_term, int days,
                          bool advance_term)
{
    int cycle = 0;
    solar_term = cal::wrap(kSolarTermCount, solar_term);
    for (int i = 0; i < days; ++i) {
        almanac_.push_back(DayInfo{cursor, solar_term, sexagenary});
        cursor = cal::add_seconds(cursor, 3600 * 24);
        ++cycle;
        sexagenary = cal::wrap(kSexagenaryCount, sexagenary + 1);
        if (cycle == 15) {
            if (advance_term) {
                solar_term = cal::wrap(kSolarTermCount, solar_term + 1);
            }
            if (solar_term == stop_term) {
                break;
            }
            cycle = 0;
        }
    }
}

std::pair<int, int> QimenV1::void_branches(int sexagenary)
{
    const int head = (sexagenary / 10) * 10;
    return {(head + 10) % kBranchCount, (head + 11) % kBranchCount};
}

}   // namespace csp
