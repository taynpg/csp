#include "qm_v2.h"

/*
星纪（丑2）：约 12月7日～1月5日（大雪～冬至～小寒前）23, 0(下标) 大雪～冬至
玄枵（子1）：约1月6日～2月3日（小寒～大寒～立春前）  1, 2 (下标) 小寒～大寒
娵訾（亥12）：约2月4日～3月5日（立春～雨水～惊蛰前） 3, 4 (下标) 立春～雨水
降娄（戌11）：约3月6日～4月4日（惊蛰～春分～清明前） 5, 6 (下标) 惊蛰～春分
大梁（酉10）：约4月5日～5月5日（清明～谷雨～立夏前） 7, 8 (下标) 清明～谷雨
实沈（申9）：约5月6日～6月5日（立夏～小满～芒种前）  9, 10(下标) 立夏～小满
鹑首（未8）：约6月6日～7月7日（芒种～夏至～小暑前）  11,12(下标) 芒种～夏至
鹑火（午7）：约7月8日～8月7日（小暑～大暑～立秋前）  13,14(下标) 小暑～大暑
鹑尾（巳6）：约8月8日～9月7日（立秋～处暑～白露前）  15,16(下标) 立秋～处暑
寿星（辰5）：约9月8日～10月7日（白露～秋分～寒露前） 17,18(下标) 白露～秋分
大火（卯4）：约10月8日～11月7日（寒露～霜降～立冬前）19,20(下标) 寒露～霜降
析木（寅3）：约11月8日～12月7日（立冬～小雪～大雪前）21,22(下标) 立冬～小雪

const char* gStr_zhi[] = {"子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"};
*/

namespace csp {

QimenV2::QimenV2()
{
}

QimenV2::~QimenV2()
{
}

bool QimenV2::generate(const DateTime& dt, int ju)
{
    if (!Qimen::set_and_check(dt, ju)) {
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
    const auto& temp_cal = data_.dt_;
    // 年支数字
    auto jiazi = Qimen::jiazi(*temp_cal);
    auto lunar = Qimen::lunar(*temp_cal);
    int yz = jiazi.yi % 12 + 1;
    // 农历月日
    int lm = lunar.mon;
    int ld = lunar.day;
    // 时辰数字
    int hm = jiazi.hi % 12 + 1;

    int sum = yz + lm + ld + hm;
    data_.ju = sum % 9;
    data_.ju = (data_.ju == 0 ? 9 : data_.ju);

    // 冬至
    DateTime dongZhi(dt.year, 12, 22, 12, 0, 0);
    auto tdz = std::make_shared<tyme::SolarTime>(dongZhi.year, dongZhi.mon, dongZhi.day, dongZhi.hour, dongZhi.min,
                                                 dongZhi.sec);
    int dzIndex{};
    dongZhi = Qimen::solar(Qimen::get_jq(*tdz, dzIndex, true));
    tdz = std::make_shared<tyme::SolarTime>(dongZhi.year, dongZhi.mon, dongZhi.day, dongZhi.hour, dongZhi.min,
                                            dongZhi.sec);

    // 夏至
    DateTime xiaZhi(dt.year, 6, 15, 12, 0, 0);
    auto txz =
        std::make_shared<tyme::SolarTime>(xiaZhi.year, xiaZhi.mon, xiaZhi.day, xiaZhi.hour, xiaZhi.min, xiaZhi.sec);
    int xzIndex{};
    xiaZhi = Qimen::solar(Qimen::get_jq(*txz, xzIndex, true));
    txz = std::make_shared<tyme::SolarTime>(xiaZhi.year, xiaZhi.mon, xiaZhi.day, xiaZhi.hour, xiaZhi.min, xiaZhi.sec);

    // 判断阴阳遁
    auto da = temp_cal->subtract(*txz);
    auto db = temp_cal->subtract(*tdz);

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
const char* gStr_jq[] = {"冬至", "小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏", "小满", "芒种",
"夏至", "小暑", "大暑", "立秋", "处暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪"
};
*/

void QimenV2::cal_yujiang()
{
    auto f = data_.dt_->get_term();
    auto after = f.get_index();
    auto index = mod(12, -((after - 1) / 2));
    data_.yuejiang_ = index;
}

}   // namespace csp
