// csp - 奇门遁甲排盘的 Python 绑定（pybind11）
#include <stdexcept>
#include <string>

#include <fmt/format.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "csp/app.hpp"
#include "csp/console.hpp"
#include "csp/version.hpp"

namespace py = pybind11;

namespace {

/// 初始化终端（仅 Windows 生效），可重复调用。
/// 只开 ANSI 支持，不接管 std::cout——嵌入到 Python 里时输出由 Python 负责。
void init_console()
{
    csp::console::enable_ansi();
}

std::string render(const csp::Options& input, bool as_json)
{
    csp::Options options = input;
    csp::Result result;
    std::string error;
    if (!csp::prepare_result(options, result, &error)) {
        throw std::runtime_error(error);
    }
    return as_json ? csp::render_json(result) : csp::render_text(result.chart, options.type, options.color);
}

}   // namespace

PYBIND11_MODULE(csp_qm, m)
{
    init_console();

    m.doc() = "奇门遁甲排盘工具 csp 的 Python 接口";
    m.attr("__version__") = csp::kVersion;
    m.def("init_console", &init_console, "初始化终端的颜色/UTF-8 输出（导入模块时已自动调用）");

    // Options 的时间是嵌套的 DateTime，这里展开成扁平属性，保持旧版 Python 接口不变
    py::class_<csp::Options>(m, "Options", "排盘参数")
        .def(py::init<>())
        .def_property(
            "year", [](const csp::Options& o) { return o.time.year; }, [](csp::Options& o, int v) { o.time.year = v; },
            "年（2-9998）")
        .def_property(
            "mon", [](const csp::Options& o) { return o.time.mon; }, [](csp::Options& o, int v) { o.time.mon = v; },
            "月（1-12）")
        .def_property(
            "day", [](const csp::Options& o) { return o.time.day; }, [](csp::Options& o, int v) { o.time.day = v; },
            "日（1-31）")
        .def_property(
            "hour", [](const csp::Options& o) { return o.time.hour; }, [](csp::Options& o, int v) { o.time.hour = v; },
            "时（0-23）")
        .def_property(
            "min", [](const csp::Options& o) { return o.time.min; }, [](csp::Options& o, int v) { o.time.min = v; },
            "分（0-59）")
        .def_property(
            "sec", [](const csp::Options& o) { return o.time.sec; }, [](csp::Options& o, int v) { o.time.sec = v; },
            "秒（0-59）")
        .def_property(
            "type", [](const csp::Options& o) { return static_cast<int>(o.type); },
            [](csp::Options& o, int v) { o.type = static_cast<csp::QimenType>(v); },
            "盘式：1 超接置润、2 阴盘、3 拆补、4 茅山")
        .def_property(
            "ju", [](const csp::Options& o) { return o.ju; }, [](csp::Options& o, int v) { o.ju = v; },
            "局数，0 为自动；负数为阴遁")
        .def_property(
            "zone", [](const csp::Options& o) { return o.zone_hours; },
            [](csp::Options& o, int v) { o.zone_hours = v; }, "与本地时间的时差（小时）")
        .def_property(
            "angan", [](const csp::Options& o) { return static_cast<int>(o.hidden_stem); },
            [](csp::Options& o, int v) { o.hidden_stem = static_cast<csp::HiddenStemStyle>(v); },
            "暗干排法：0 门原宫位地盘、1 入值使门")
        .def_property(
            "is_auto", [](const csp::Options& o) { return o.auto_time; },
            [](csp::Options& o, bool v) { o.auto_time = v; }, "是否使用当前时间")
        .def_property(
            "color", [](const csp::Options& o) { return o.color; }, [](csp::Options& o, bool v) { o.color = v; },
            "终端输出是否着色")
        .def_readwrite("str_dt", &csp::Options::date_text, "日期原文（仅用于回显）")
        .def_readwrite("export_json", &csp::Options::export_json, "是否导出 JSON 文件")
        .def("__repr__", [](const csp::Options& o) {
            return fmt::format("Options({:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}, type={}, ju={}, zone={}, "
                               "angan={}, is_auto={})",
                               o.time.year, o.time.mon, o.time.day, o.time.hour, o.time.min, o.time.sec,
                               static_cast<int>(o.type), o.ju, o.zone_hours, static_cast<int>(o.hidden_stem),
                               o.auto_time);
        });

    py::class_<csp::Runner>(m, "Runner", "排盘并打印结果")
        .def(py::init<>())
        .def("run", &csp::Runner::run, "排盘并打印（可选导出 JSON）", py::arg("options"));

    m.def(
        "render", [](const csp::Options& options) { return render(options, false); }, "排盘并返回终端文本（不打印）",
        py::arg("options"));
    m.def(
        "render_json", [](const csp::Options& options) { return render(options, true); }, "排盘并返回 JSON 文本",
        py::arg("options"));

    // 旧版名字，保持向后兼容
    m.attr("CmdParam") = m.attr("Options");
    m.attr("CQimenUse") = m.attr("Runner");
}
