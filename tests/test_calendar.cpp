#include "test_framework.hpp"

#include "csp/calendar.hpp"
#include "csp/zh.hpp"

using namespace csp;

// wrap 的结果恒为非负
CSP_TEST(wrap_is_always_non_negative)
{
    CHECK_EQ(cal::wrap(9, 0), 0);
    CHECK_EQ(cal::wrap(9, 9), 0);
    CHECK_EQ(cal::wrap(9, -1), 8);
    CHECK_EQ(cal::wrap(24, -1), 23);
    CHECK_EQ(cal::wrap(60, -20), 40);
}

// 1986-05-29 12:00 的四柱为 丙寅 癸巳 癸酉 戊午
CSP_TEST(four_pillars_match_known_chart)
{
    const auto time = cal::to_solar(DateTime(1986, 5, 29, 12, 0, 0));
    const auto pillars = cal::four_pillars(time);

    CHECK_EQ(zh::sexagenary(pillars.year), "丙寅");
    CHECK_EQ(zh::sexagenary(pillars.month), "癸巳");
    CHECK_EQ(zh::sexagenary(pillars.day), "癸酉");
    CHECK_EQ(zh::sexagenary(pillars.hour), "戊午");
}

// 10 月的相邻节气为寒露与霜降
CSP_TEST(terms_around_october)
{
    const auto span = cal::terms_around(cal::to_solar(DateTime(2026, 10, 19, 8, 25, 0)));

    CHECK_EQ(span.previous.index, 19);
    CHECK_EQ(zh::solar_term(span.previous.index), "寒露");
    CHECK_EQ(span.next.index, 20);
    CHECK_EQ(zh::solar_term(span.next.index), "霜降");

    const auto previous = cal::from_solar(span.previous.time);
    CHECK_EQ(previous.mon, 10);
    CHECK_EQ(previous.day, 8);
    CHECK_EQ(previous.hour, 14);
    CHECK_EQ(previous.min, 29);
    CHECK_EQ(previous.sec, 17);

    const auto next = cal::from_solar(span.next.time);
    CHECK_EQ(next.mon, 10);
    CHECK_EQ(next.day, 23);
}

// 12 月的下一个节气序号为 24，按约定折算为次年 0 号（冬至）
CSP_TEST(terms_cross_year_at_december)
{
    const auto span = cal::terms_around(cal::to_solar(DateTime(2026, 12, 10, 12, 0, 0)));
    CHECK_EQ(zh::solar_term(span.previous.index), "大雪");
    CHECK_EQ(span.next.index, 0);
    CHECK_EQ(zh::solar_term(span.next.index), "冬至");
}

CSP_TEST(add_seconds_and_days_between)
{
    const auto shifted = cal::add_seconds(DateTime(2026, 12, 31, 23, 59, 59), 1);
    CHECK_EQ(shifted.year, 2027);
    CHECK_EQ(shifted.mon, 1);
    CHECK_EQ(shifted.day, 1);
    CHECK_EQ(shifted.hour, 0);
    CHECK_EQ(shifted.min, 0);
    CHECK_EQ(shifted.sec, 0);

    CHECK_EQ(cal::days_between(DateTime(2026, 1, 1, 0, 0, 0), DateTime(2026, 1, 31, 0, 0, 0)), 30);
}
