#include "qmuse.h"
#include "jsonExport.h"
#include <fmt/format.h>

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
        dt = Qimen::now_time();
    }

    if (param.type < 1 || param.type > 4) {
        fmt::print("不支持的盘类型范围，应当在[1,4]。\n");
        return false;
    }

    if (param.zone != 0) {
        auto pt = tyme::SolarTime(dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec);
        pt = pt.next(param.zone * 3600);
        dt = Qimen::solar(pt);
    }

    auto qm_type = static_cast<QimenType>(param.type);
    auto qm = Qimen::instance(qm_type);

    // 时间偏移

    if (!qm->generate(dt, param.ju, param.angan)) {
        return false;
    }
    print_.run(qm, param.type);

    JsonExport je;
    if (param.exportJson) {
        if (!je.exportJson(qm, param.type)) {
            fmt::print("导出JSON失败。\n");
            return false;
        }
    }

    return true;
}
