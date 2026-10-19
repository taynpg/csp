#include "csp/qimen_variants.hpp"

#include "csp/calendar.hpp"

namespace csp {

bool QimenV2::resolve_ju(const DateTime& dt)
{
    if (chart_.ju != 0) {
        // 手动定局：阴阳遁已由局数的正负决定，阴盘没有三元的概念
        chart_.yuan = Yuan::Manual;
        return true;
    }
    if (!compute_ju(dt)) {
        return false;
    }
    chart_.yuan = Yuan::Auto;
    return true;
}

bool QimenV2::compute_ju(const DateTime& dt)
{
    const auto& p = pillars();
    const DateTime lunar = cal::from_lunar(time());

    // 年支数 + 农历月 + 农历日 + 时支数，取九余
    const int sum = (p.year % kBranchCount + 1) + lunar.mon + lunar.day + (p.hour % kBranchCount + 1);
    chart_.ju = sum % kPalaceCount;
    if (chart_.ju == 0) {
        chart_.ju = kPalaceCount;
    }

    // 12 月 22 日之后的下一个节气即冬至；6 月 15 日之后的下一个节气即夏至
    const auto winter = cal::terms_around(cal::to_solar(DateTime(dt.year, 12, 22, 12, 0, 0))).next.time;
    const auto summer = cal::terms_around(cal::to_solar(DateTime(dt.year, 6, 15, 12, 0, 0))).next.time;

    const int since_summer = time().subtract(summer);
    const int since_winter = time().subtract(winter);
    // 夏至之后、冬至之前为阴遁
    chart_.yin_dun = (since_summer > 0 && since_winter <= 0);

    update_moon_general();
    return true;
}

void QimenV2::update_moon_general()
{
    // 月将按节气两两一组，自冬至起逆行
    const auto term = time().get_term();
    chart_.moon_general = cal::wrap(kBranchCount, -((term.get_index() - 1) / 2));
}

}   // namespace csp
