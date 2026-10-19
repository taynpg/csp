#include "csp/qimen_variants.hpp"

#include <algorithm>

#include "csp/calendar.hpp"
#include "ju_pattern.hpp"

namespace csp {

Yuan QimenV4::yuan_of(int seconds_since_term)
{
    constexpr int kFiveDays = 3600 * 24 * 5;
    constexpr int kTenDays = kFiveDays * 2;

    if (seconds_since_term < kFiveDays) {
        return Yuan::Upper;
    }
    if (seconds_since_term < kTenDays) {
        return Yuan::Middle;
    }
    return Yuan::Lower;
}

bool QimenV4::compute_ju(const DateTime&)
{
    const auto& t = time();
    const auto span = cal::terms_around(t);
    const int since_previous = t.subtract(span.previous.time);
    const int since_next = t.subtract(span.next.time);

    // 上个月的节气，用于月初（本月节气之前）的情形。
    // 取上个月的同一天；上个月天数更少时取该月最后一天，否则会构造出 11 月 31 日这类非法日期。
    const auto previous_month = tyme::SolarMonth::from_ym(t.get_year(), t.get_month()).next(-1);
    const int previous_day = std::min(t.get_day(), previous_month.get_day_count());
    const auto previous_time = tyme::SolarTime(previous_month.get_year(), previous_month.get_month(), previous_day,
                                               t.get_hour(), t.get_minute(), t.get_second());
    const auto previous_span = cal::terms_around(previous_time);

    const cal::Term* term = nullptr;
    if (since_previous < 0) {
        term = &previous_span.next;
    } else if (since_next < 0) {
        term = &span.previous;
    } else {
        term = &span.next;
    }
    chart_.solar_term = term->index;

    // 三元由交节后的天数决定
    chart_.yuan = yuan_of(t.subtract(term->time));

    const auto& pattern = detail::kJuPatterns[chart_.solar_term];
    chart_.yin_dun = !pattern.yang;
    chart_.ju = detail::ju_of(pattern, chart_.yuan);
    return true;
}

}   // namespace csp
