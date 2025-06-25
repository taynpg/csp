#include "qm_v2.h"

/*
星纪（丑）：约 12月7日～1月5日（大雪～冬至～小寒前）
玄枵（子）：约1月6日～2月3日（小寒～大寒～立春前）
娵訾（亥）：约2月4日～3月5日（立春～雨水～惊蛰前）
降娄（戌）：约3月6日～4月4日（惊蛰～春分～清明前）
大梁（酉）：约4月5日～5月5日（清明～谷雨～立夏前）
实沈（申）：约5月6日～6月5日（立夏～小满～芒种前）
鹑首（未）：约6月6日～7月7日（芒种～夏至～小暑前）
鹑火（午）：约7月8日～8月7日（小暑～大暑～立秋前）
鹑尾（巳）：约8月8日～9月7日（立秋～处暑～白露前）
寿星（辰）：约9月8日～10月7日（白露～秋分～寒露前）
大火（卯）：约10月8日～11月7日（寒露～霜降～立冬前）
析木（寅）：约11月8日～12月7日（立冬～小雪～大雪前）
*/

namespace csp {

QimenV2::QimenV2()
{
}

QimenV2::~QimenV2()
{
}

bool QimenV2::generate(const DateTime& dt, int ju, CalendarType ct)
{
    if (!Qimen::set_and_check(dt, ju, ct)) {
        return false;
    }
    if (!cal_->generate(dt_)) {
        return false;
    }

    if (data_.ju == 0) {
        if (!cal_ju(dt)) {
            return false;
        }
        data_.yuan = -1;
    } else {
        data_.yuan = 0;
        if (data_.ju < 0) {
            data_.is_yin = true;
        } else {
            data_.is_yin = false;
        }
        data_.ju = std::abs(data_.ju);
    }
    cal_dipan();
    cal_duty();
    cal_jiuxing();
    cal_bamen();
    cal_bashen();
    cal_tianpan();
    cal_other();

    return true;
}

bool QimenV2::cal_ju(const DateTime& dt)
{
    auto tcal = Calendar::instance(ctype_);
    if (!tcal->generate(dt)) {
        return false;
    }
    // 年支数字
    int yz = cal_->get_ganzhi().yz + 1;
    // 农历月日
    int lm = cal_->get_lunar().mon;
    int ld = cal_->get_lunar().day;
    // 时辰数字
    int hm = cal_->get_ganzhi().hz + 1;

    int sum = yz + lm + ld + hm;
    data_.ju = sum % 9;
    data_.ju = (data_.ju == 0 ? 9 : data_.ju);

    // 判断阴阳遁
    // *************** 冬至
    DateTime dz(dt.year, 12, 12, 12, 0, 0);
    if (!tcal->generate(dz)) {
        return false;
    }
    dz = tcal->get_jq_info()[3].dt;
    // *************** 夏至
    DateTime xz(dt.year, 6, 15, 12, 0, 0);
    if (!tcal->generate(xz)) {
        return false;
    }
    xz = tcal->get_jq_info()[3].dt;

    auto da = cal_->calsecs_by_all(dt, xz);
    auto db = cal_->calsecs_by_all(dt, dz);

    // --- 阳遁 --- 夏至 --- 阴遁 --- 冬至 --- 阳遁
    if (da > 0 && db <= 0) {
        data_.is_yin = true;
    } else {
        data_.is_yin = false;
    }
    cal_yujiang();
    return true;
}

/*

const char* gStr_jq[] = {"小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏", "小满", "芒种", "夏至",
                         "小暑", "大暑", "立秋", "处暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪", "冬至"};
*/

void QimenV2::cal_yujiang()
{
    auto jq = cal_->get_jq_info();
    auto da = cal_->calsecs_by_all(cal_->get_solar(), jq[2].dt);
    auto db = cal_->calsecs_by_all(cal_->get_solar(), jq[3].dt);

    // -- 节气1 --- 节气2 ---
    int ji = -1;
    if (da < 0) {
        ji = jq[1].index;
    }
    else {
        ji = jq[2].index;
    }
    data_.yuejiang_ = 12 - ji / 2;
    int a = 0;
}

}   // namespace csp
