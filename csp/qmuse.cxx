#include "qmuse.h"
#include <calendar.h>
#include <iostream>

using namespace csp;

bool CQimenUse::run(const CmdParam& param)
{
    DateTime dt{};
    if (!param.is_auto) {
        dt.year = param.year;
        dt.mon = param.mon;
        dt.day = param.day;
        dt.hour = param.hour;
        dt.min = param.min;
        dt.sec = param.sec;
    } else {
        dt = Calendar::now();
    }

    if (param.type < 1 || param.type > 4) {
        std::cout << "不支持的盘类型范围，应当在[1,4]。" << std::endl;
        return false;
    }
    if (param.cal_type < 0 || param.cal_type > 1) {
        std::cout << "不支持的日历类型范围，应当在[0,1]。" << std::endl;
        return false;
    }

    auto cal_type = static_cast<CalendarType>(param.cal_type);
    auto qm_type = static_cast<QimenType>(param.type);
    auto qm = Qimen::instance(qm_type);

    if (param.zone != 0) {
        auto pt = Calendar::instance(cal_type);
        dt = pt->cal_new_date(dt, param.zone * 3600);
    }
    if (!qm->generate(dt, param.ju, cal_type)) {
        std::cout << qm->get_error() << std::endl;
        return false;
    }
    print_.run(qm, param.type);
    return true;
}
