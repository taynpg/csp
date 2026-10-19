#include <filesystem>
#include <fstream>
#include <string>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include "csp/app.hpp"
#include "csp/calendar.hpp"
#include "csp/version.hpp"
#include "csp/zh.hpp"

namespace csp {

namespace {

using json = nlohmann::json;

std::string to_datetime_str(const DateTime& dt)
{
    return fmt::format("{:0>4}-{:0>2}-{:0>2} {:0>2}:{:0>2}:{:0>2}", dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec);
}

/// 宫位 -> 宫名，如 坎一宫、中五宫
std::string palace_name(const Chart& chart, int palace)
{
    const int trigram = chart.palace_to_trigram[palace];
    return fmt::format("{}{}宫", zh::trigram(trigram), zh::numeral(trigram + 1));
}

std::string text_of(std::string_view value)
{
    return std::string(value);
}

}   // namespace

std::string render_json(const Result& result)
{
    const Chart& chart = result.chart;
    const auto& time = *chart.time;
    const auto solar = cal::from_solar(time);
    const auto lunar = cal::from_lunar(time);
    const auto pillars = cal::four_pillars(time);
    const auto type = result.options.type;

    json j;

    // ==================== 基本信息 ====================
    json& meta = j["基本信息"];
    meta["工具"] = "csp";
    meta["版本"] = kVersion;
    meta["盘式类型"] = static_cast<int>(type);
    meta["盘式名称"] = text_of(zh::plate_name(type));
    meta["公历"] = to_datetime_str(solar);
    meta["农历"] = {
        {"年", lunar.year},
        {"月", zh::month(lunar.mon - 1)},
        {"闰月", lunar.mon < 0},
        {"日", text_of(zh::lunar_day(lunar.day - 1))},
        {"时辰", text_of(zh::earth_branch((lunar.hour + 1) / 2 % 12))},
    };
    meta["四柱"] = {
        {"年柱", text_of(zh::sexagenary(pillars.year))},
        {"月柱", text_of(zh::sexagenary(pillars.month))},
        {"日柱", text_of(zh::sexagenary(pillars.day))},
        {"时柱", text_of(zh::sexagenary(pillars.hour))},
    };
    meta["当日节气"] = text_of(zh::solar_term(chart.solar_term));

    const auto yuan = zh::yuan_name(chart.yuan);
    meta["三元"] = (chart.yuan == Yuan::Auto || chart.yuan == Yuan::Manual)
                       ? std::string(yuan)
                       : text_of(zh::solar_term(chart.solar_term)) + std::string(yuan);

    meta["阴阳遁"] = chart.yin_dun ? "阴遁" : "阳遁";
    meta["局数"] = chart.ju;
    meta["值符星"] = text_of(zh::star(chart.stars_origin[chart.duty_palace]));
    meta["值使门"] = text_of(zh::gate(chart.gates_origin[chart.duty_palace]));
    meta["五不遇时"] = chart.wu_bu_yu;
    if (type == QimenType::YinPan) {
        meta["月将"] = text_of(zh::earth_branch(chart.moon_general));
    }

    const auto span = cal::terms_around(time);
    meta["上一节气"] = {{"名称", text_of(zh::solar_term(span.previous.index))},
                        {"时间", to_datetime_str(cal::from_solar(span.previous.time))}};
    meta["下一节气"] = {{"名称", text_of(zh::solar_term(span.next.index))},
                        {"时间", to_datetime_str(cal::from_solar(span.next.time))}};

    // ==================== 旬空（四柱各一组空亡地支） ====================
    static const char* kPillarNames[4] = {"年柱", "月柱", "日柱", "时柱"};
    json voids = json::object();
    for (int i = 0; i < 4; ++i) {
        voids[kPillarNames[i]] = {text_of(zh::earth_branch(chart.void_branch[i][0])),
                                  text_of(zh::earth_branch(chart.void_branch[i][1]))};
    }
    j["旬空"] = voids;

    // ==================== 九宫 ====================
    json palaces = json::array();
    for (int palace = 0; palace < kPalaceCount; ++palace) {
        json item;
        item["宫位"] = palace_name(chart, palace);
        item["五行"] = text_of(zh::element(chart.element[palace]));
        item["九星"] = text_of(zh::star(chart.stars[palace]));
        item["天盘"] = text_of(zh::heaven_stem(chart.heaven_stem[palace]));
        item["地盘"] = text_of(zh::heaven_stem(chart.earth_stem[palace]));
        if (palace == kCenterPalace) {
            // 中宫无门无神，天禽寄于寄宫
            item["八门"] = nullptr;
            item["八神"] = nullptr;
            item["暗干"] = nullptr;
            item["寄往宫位"] = palace_name(chart, chart.lodging_palace);
        } else {
            item["八门"] = text_of(zh::gate(chart.gates[palace]));
            item["八神"] = text_of(zh::deity(chart.deities[palace]));
            item["暗干"] = text_of(zh::heaven_stem(chart.hidden_stem[palace]));
        }
        item["天禽寄宫"] = (palace == chart.lodging_palace);
        item["马星"] = (palace == chart.horse_palace);
        item["空亡"] = (palace == chart.void_palace[0] || palace == chart.void_palace[1]);
        palaces.push_back(item);
    }
    j["九宫"] = palaces;

    return j.dump(4);
}

bool write_json_file(const Result& result, std::string* file_path, std::string* error)
{
    const auto solar = cal::from_solar(*result.chart.time);

    // 按排盘时间命名（文件名中不能含冒号），输出到运行时工作目录
    const std::string name = fmt::format("csp_{:0>4}-{:0>2}-{:0>2}_{:0>2}-{:0>2}-{:0>2}.json", solar.year, solar.mon,
                                         solar.day, solar.hour, solar.min, solar.sec);

    std::error_code code;
    const auto path = std::filesystem::current_path(code) / name;
    if (code) {
        if (error != nullptr) {
            *error = code.message();
        }
        return false;
    }

    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        if (error != nullptr) {
            *error = "无法创建文件 " + path.string();
        }
        return false;
    }
    file << render_json(result);
    file.close();
    if (file.fail()) {
        if (error != nullptr) {
            *error = "写入文件失败 " + path.string();
        }
        return false;
    }

    if (file_path != nullptr) {
        *file_path = path.string();
    }
    return true;
}

}   // namespace csp
