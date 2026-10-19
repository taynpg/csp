// 文本与 JSON 快照测试：改动排盘或渲染后，输出若发生变化会在这里暴露。
#include "test_framework.hpp"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#include "csp/app.hpp"

#ifndef CSP_SNAPSHOT_DIR
#define CSP_SNAPSHOT_DIR "snapshot"
#endif

using namespace csp;

namespace {

struct SnapCase {
    const char* name;
    QimenType type;
    DateTime time;
    int ju;
    HiddenStemStyle style;
    bool as_json;
};

const SnapCase kCases[] = {
    {"t1_1986-05-29_12-00-00", QimenType::ZhiRun, DateTime(1986, 5, 29, 12, 0, 0), 0, HiddenStemStyle::ByOriginalPalace,
     false},
    {"t1_2026-10-19_08-25-00", QimenType::ZhiRun, DateTime(2026, 10, 19, 8, 25, 0), 0,
     HiddenStemStyle::ByOriginalPalace, false},
    {"t1_2026-10-19_angan1", QimenType::ZhiRun, DateTime(2026, 10, 19, 8, 25, 0), 0, HiddenStemStyle::ByDutyGate,
     false},
    {"t1_mao_hour", QimenType::ZhiRun, DateTime(2026, 3, 15, 5, 30, 0), 0, HiddenStemStyle::ByOriginalPalace, false},
    {"t2_1986-05-29_12-00-00", QimenType::YinPan, DateTime(1986, 5, 29, 12, 0, 0), 0, HiddenStemStyle::ByOriginalPalace,
     false},
    {"t3_1986-05-29_12-00-00", QimenType::ChaiBu, DateTime(1986, 5, 29, 12, 0, 0), 0, HiddenStemStyle::ByOriginalPalace,
     false},
    {"t4_1986-05-29_12-00-00", QimenType::MaoShan, DateTime(1986, 5, 29, 12, 0, 0), 0,
     HiddenStemStyle::ByOriginalPalace, false},
    {"t1_manual_ju", QimenType::ZhiRun, DateTime(2026, 10, 19, 8, 25, 0), -5, HiddenStemStyle::ByOriginalPalace, false},
    {"t1_1900-02-28_23-59-59", QimenType::ZhiRun, DateTime(1900, 2, 28, 23, 59, 59), 0,
     HiddenStemStyle::ByOriginalPalace, false},
    {"t1_2100-12-31_23-00-00", QimenType::ZhiRun, DateTime(2100, 12, 31, 23, 0, 0), 0,
     HiddenStemStyle::ByOriginalPalace, false},
    {"t1_1986-05-29_12-00-00", QimenType::ZhiRun, DateTime(1986, 5, 29, 12, 0, 0), 0, HiddenStemStyle::ByOriginalPalace,
     true},
};

bool update_mode()
{
    const char* value = std::getenv("CSP_UPDATE_SNAPSHOTS");
    return value != nullptr && std::string(value) != "0";
}

std::string path_of(const SnapCase& item)
{
    return std::string(CSP_SNAPSHOT_DIR) + "/" + item.name + (item.as_json ? ".json" : ".txt");
}

bool read_file(const std::string& path, std::string& content)
{
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return false;
    }
    std::ostringstream stream;
    stream << file.rdbuf();
    content = stream.str();
    // 快照以 LF 保存；若检出时被换行符转换过，这里统一去掉 CR
    content.erase(std::remove(content.begin(), content.end(), '\r'), content.end());
    return true;
}

void write_file(const std::string& path, const std::string& content)
{
    std::ofstream file(path, std::ios::binary);
    file << content;
}

/// 找第一处差异，便于定位
std::string describe_difference(const std::string& actual, const std::string& expected)
{
    std::size_t index = 0;
    while (index < actual.size() && index < expected.size() && actual[index] == expected[index]) {
        ++index;
    }
    return "第 " + std::to_string(index) + " 字节起不一致（实际 " + std::to_string(actual.size()) + " 字节，快照 " +
           std::to_string(expected.size()) + " 字节）";
}

}   // namespace

CSP_TEST(snapshot_matches)
{
    for (const auto& item : kCases) {
        Options options;
        options.type = item.type;
        options.time = item.time;
        options.ju = item.ju;
        options.hidden_stem = item.style;

        Result result;
        CHECK(build_chart(options, result));
        if (result.chart.time == std::nullopt) {
            continue;
        }

        const std::string actual = item.as_json ? render_json(result) : render_text(result.chart, options.type, false);
        const std::string path = path_of(item);

        if (update_mode()) {
            write_file(path, actual);
            continue;
        }

        std::string expected;
        if (!read_file(path, expected)) {
            testing::report_failure(__FILE__, __LINE__,
                                    std::string("缺少快照 ") + path + "（可用 CSP_UPDATE_SNAPSHOTS=1 生成）");
            continue;
        }
        if (actual != expected) {
            testing::report_failure(__FILE__, __LINE__,
                                    std::string("快照不一致 ") + path + "，" + describe_difference(actual, expected));
        }
    }
}

CSP_TEST(colored_output_contains_ansi_codes)
{
    Options options;
    options.type = QimenType::ZhiRun;
    options.time = DateTime(2026, 10, 19, 8, 25, 0);

    Result result;
    CHECK(build_chart(options, result));
    const auto colored = render_text(result.chart, options.type, true);
    const auto plain = render_text(result.chart, options.type, false);

    CHECK(colored.find("\x1b[") != std::string::npos);
    CHECK(plain.find("\x1b[") == std::string::npos);
    CHECK(colored.size() > plain.size());
}
