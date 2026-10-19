#include "csp/qimen_variants.hpp"

#include "csp/calendar.hpp"
#include "ju_pattern.hpp"

namespace csp {

bool QimenV3::compute_ju(const DateTime&)
{
    const auto& t = time();
    const auto& p = pillars();

    // 符头只有甲、己两个
    const int head = p.day - p.day % 5;
    const int branch_in_cycle = head % kBranchCount;

    // 找当前所在的节气：上半月在本月节气之前，下半月在本月节气之后
    const auto span = cal::terms_around(t);
    const int since_previous = t.subtract(span.previous.time);
    const int since_next = t.subtract(span.next.time);

    if (since_previous >= 0 && since_next < 0) {
        chart_.solar_term = span.previous.index;
    } else if (since_previous < 0) {
        chart_.solar_term = cal::wrap(kSolarTermCount, span.previous.index - 1);
    } else {
        chart_.solar_term = span.next.index;
    }

    const auto& pattern = detail::kJuPatterns[chart_.solar_term];
    chart_.yin_dun = !pattern.yang;

    // 符头地支 0/3/6/9 为上元，2/5/8/11 为中元，1/4/7/10 为下元
    switch (branch_in_cycle % 3) {
    case 0:
        chart_.yuan = Yuan::Upper;
        break;
    case 1:
        chart_.yuan = Yuan::Lower;
        break;
    default:
        chart_.yuan = Yuan::Middle;
        break;
    }
    chart_.ju = detail::ju_of(pattern, chart_.yuan);
    return true;
}

}   // namespace csp
