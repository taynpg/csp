#include "jsonExport.h"

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>

#include "csp_base.hpp"
#include "zh_lang.h"

using json = nlohmann::json;
using namespace csp;

namespace {

// 时间转字符串 0000-00-00 00:00:00
std::string to_datetime_str(const csp::DateTime& dt)
{
    return fmt::format("{:0>4}-{:0>2}-{:0>2} {:0>2}:{:0>2}:{:0>2}", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec);
}

// 宫位索引转宫位名称（如：坎一宫、中五宫）
std::string gong_name(const csp::QimenData& qmd, int g)
{
    int guaIdx = qmd.pos2gua[g];
    return CZhData::gua(guaIdx) + CZhData::num(guaIdx + 1) + "宫";
}

}   // namespace

bool JsonExport::exportJson(const std::shared_ptr<csp::Qimen>& qm, int type)
{
    using namespace csp;

    auto qmd = qm->get_result();
    auto cal = qmd.dt_;
    const auto& solar = Qimen::solar(*cal);
    const auto& lunar = Qimen::lunar(*cal);
    const auto& gz = Qimen::jiazi(*cal);

    json j;

    // ==================== 基本信息 ====================
    json& meta = j["基本信息"];
    meta["工具"] = "csp";
    meta["版本"] = CSP_VERSION;
    meta["盘式类型"] = type;
    switch (type) {
    case 1:
        meta["盘式名称"] = "时家转盘超接置润法";
        break;
    case 2:
        meta["盘式名称"] = "时家阴盘";
        break;
    case 3:
        meta["盘式名称"] = "时家转盘拆补法";
        break;
    case 4:
        meta["盘式名称"] = "时家转盘茅山法";
        break;
    default:
        break;
    }
    meta["公历"] = to_datetime_str(solar);
    meta["农历"] = {
        {"年", lunar.year},
        {"月", CZhData::mon(lunar.mon - 1)},
        {"闰月", lunar.mon < 0},
        {"日", CZhData::lunar_day(lunar.day - 1)},
        {"时辰", CZhData::zhi((lunar.hour + 1) / 2 % 12)},
    };
    meta["四柱"] = {
        {"年柱", CZhData::gan(gz.yi % 10) + CZhData::zhi(gz.yi % 12)},
        {"月柱", CZhData::gan(gz.mi % 10) + CZhData::zhi(gz.mi % 12)},
        {"日柱", CZhData::gan(gz.di % 10) + CZhData::zhi(gz.di % 12)},
        {"时柱", CZhData::gan(gz.hi % 10) + CZhData::zhi(gz.hi % 12)},
    };
    meta["当日节气"] = CZhData::jq(qmd.jieq);

    std::string szYuan;
    switch (qmd.yuan) {
    case -1:
        szYuan = "自动定局";
        break;
    case 0:
        szYuan = "手动定局";
        break;
    case 1:
        szYuan = "下元";
        break;
    case 2:
        szYuan = "中元";
        break;
    case 3:
        szYuan = "上元";
        break;
    default:
        break;
    }
    if (qmd.yuan == 1 || qmd.yuan == 2 || qmd.yuan == 3) {
        szYuan = CZhData::jq(qmd.jieq) + szYuan;
    }
    meta["三元"] = szYuan;

    meta["阴阳遁"] = qmd.is_yin ? "阴遁" : "阳遁";
    meta["局数"] = qmd.ju;
    meta["值符星"] = CZhData::jx(qmd.jiuxp[qmd.duty]);
    meta["值使门"] = CZhData::bm(qmd.bamenp[qmd.duty]);
    meta["五不遇时"] = qmd.wubuyu;
    if (type == 2) {
        meta["月将"] = CZhData::zhi(qmd.yuejiang_);
    }

    int firstIndex{};
    int secondIndex{};
    auto fq = Qimen::get_jq(*qmd.dt_, firstIndex, false);
    auto sq = Qimen::get_jq(*qmd.dt_, secondIndex, true);
    meta["上一节气"] = {{"名称", CZhData::jq(firstIndex)}, {"时间", to_datetime_str(Qimen::solar(fq))}};
    meta["下一节气"] = {{"名称", CZhData::jq(secondIndex)}, {"时间", to_datetime_str(Qimen::solar(sq))}};

    // ==================== 旬空（四柱各一组的空亡地支） ====================
    j["旬空"] = {
        {"年柱", {CZhData::zhi(qmd.xunkong[0]), CZhData::zhi(qmd.xunkong[1])}},
        {"月柱", {CZhData::zhi(qmd.xunkong[2]), CZhData::zhi(qmd.xunkong[3])}},
        {"日柱", {CZhData::zhi(qmd.xunkong[4]), CZhData::zhi(qmd.xunkong[5])}},
        {"时柱", {CZhData::zhi(qmd.xunkong[6]), CZhData::zhi(qmd.xunkong[7])}},
    };

    // ==================== 九宫 ====================
    static const char* wuxingNames[] = {"", "金", "水", "木", "火", "土"};
    json gongs = json::array();
    for (int g = 0; g < gn; ++g) {
        json gong;
        gong["宫位"] = gong_name(qmd, g);
        gong["五行"] = wuxingNames[qmd.wuxing[g]];
        gong["九星"] = CZhData::jx(qmd.jiuxr[g]);
        gong["天盘"] = CZhData::gan(qmd.tp[g]);
        gong["地盘"] = CZhData::gan(qmd.dp[g]);
        if (g == 8) {
            // 中宫无门无神，天禽寄于寄宫
            gong["八门"] = nullptr;
            gong["八神"] = nullptr;
            gong["暗干"] = nullptr;
            gong["寄往宫位"] = gong_name(qmd, qmd.jigong);
        } else {
            gong["八门"] = CZhData::bm(qmd.bamenr[g]);
            gong["八神"] = CZhData::bs(qmd.bashenr[g]);
            gong["暗干"] = CZhData::gan(qmd.angan[g]);
        }
        gong["天禽寄宫"] = (g == qmd.jigong);
        gong["马星"] = (g == qmd.maxing);
        gong["空亡"] = (g == qmd.kongw[0] || g == qmd.kongw[1]);
        gongs.push_back(gong);
    }
    j["九宫"] = gongs;

    // 按排盘时间命名文件（文件名中不能含冒号，用减号代替），输出到运行时工作目录
    std::string basename = fmt::format("csp_{:0>4}-{:0>2}-{:0>2}_{:0>2}-{:0>2}-{:0>2}.json", solar.year, solar.mon,
                                       solar.day, solar.hour, solar.min, solar.sec);
    std::filesystem::path filepath = std::filesystem::current_path() / basename;

    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    file << j.dump(4);
    file.close();
    if (file.fail()) {
        return false;
    }

    fmt::print("结果已导出到: {}\n", filepath.string());
    return true;
}
