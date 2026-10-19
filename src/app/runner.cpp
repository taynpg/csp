#include "csp/app.hpp"

#include <exception>
#include <string>

#include <fmt/format.h>

#include "csp/calendar.hpp"
#include "csp/console.hpp"
#include "csp/qimen.hpp"

namespace csp {

bool build_chart(const Options& options, Result& result, std::string* error)
{
    const auto qimen = Qimen::create(options.type);
    if (!qimen) {
        if (error != nullptr) {
            *error = "未匹配到支持的盘面类型，请使用 --help 查看帮助";
        }
        return false;
    }

    try {
        if (!qimen->generate(options.time, options.ju, options.hidden_stem)) {
            if (error != nullptr) {
                *error = qimen->error().empty() ? "排盘失败" : qimen->error();
            }
            return false;
        }
    } catch (const std::exception& e) {
        // 日历库抛出的非法日期等问题不能让它逃出去终止进程
        if (error != nullptr) {
            *error = fmt::format("排盘失败：{}", e.what());
        }
        return false;
    }

    result.chart = qimen->chart();
    result.options = options;
    return true;
}

bool prepare_result(Options& options, Result& result, std::string* error)
{
    std::string reason;
    if (!validate(options, &reason)) {
        if (error != nullptr) {
            *error = reason;
        }
        return false;
    }

    // 时差偏移
    if (options.zone_hours != 0) {
        try {
            options.time = cal::add_seconds(options.time, options.zone_hours * 3600);
        } catch (const std::exception& e) {
            if (error != nullptr) {
                *error = fmt::format("时间偏移后超出支持范围：{}", e.what());
            }
            return false;
        }
    }

    if (!build_chart(options, result, error)) {
        if (error != nullptr && error->empty()) {
            *error = "排盘失败";
        }
        return false;
    }

    // 回填自动定局的结果，方便调用方读取
    result.options.ju = result.chart.ju;
    options.ju = result.chart.ju;
    return true;
}

bool Runner::run(Options& options)
{
    std::string error;
    Result result;
    if (!prepare_result(options, result, &error)) {
        console::write_error(error + "\n");
        return false;
    }
    result_ = result;

    console::write(render_text(result.chart, result.options.type, result.options.color));

    if (options.export_json) {
        std::string path;
        if (!write_json_file(*result_, &path, &error)) {
            console::write_error(fmt::format("导出 JSON 失败：{}\n", error));
            return false;
        }
        console::write(fmt::format("结果已导出到: {}\n", path));
    }
    return true;
}

}   // namespace csp
