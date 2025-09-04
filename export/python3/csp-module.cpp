#include "../../src/csp_base.hpp"
#include "../../src/qmuse.h"
#include "pybind11/pybind11.h"
#include <fmt/format.h>
#include <iostream>

namespace py = pybind11;

PYBIND11_MODULE(csp, m)
{
    // 模块文档
    m.doc() = "奇门遁甲排盘工具 CSP 的 Python 接口";

    // 绑定 CmdParam 结构体（带详细说明）
    py::class_<CmdParam>(m, "CmdParam", "奇门排盘参数结构体")
        .def(py::init<>(), "默认构造函数")
        // 成员变量（带说明）
        .def_readwrite("year", &CmdParam::year, "年（如 2023）")
        .def_readwrite("mon", &CmdParam::mon, "月（1-12）")
        .def_readwrite("day", &CmdParam::day, "日（1-31）")
        .def_readwrite("hour", &CmdParam::hour, "时（0-23）")
        .def_readwrite("min", &CmdParam::min, "分（0-59）")
        .def_readwrite("sec", &CmdParam::sec, "秒（0-59）")
        .def_readwrite("ju", &CmdParam::ju, "局数（自动计算时忽略）")
        .def_readwrite("type", &CmdParam::type,
                       "盘式类型（必填项）：\n"
                       "1=时家转盘超接置润\n"
                       "2=时家转盘阴盘\n"
                       "3=时家转盘拆补\n"
                       "4=时家茅山")
        .def_readwrite("zone", &CmdParam::zone, "时区偏移（小时，默认为0）")
        .def_readwrite("is_auto", &CmdParam::is_auto, "是否自动计算时间（true/false）")
        .def_readwrite("str_dt", &CmdParam::str_dt, "手动日期字符串（格式：YYYY-MM-DD-HH-MM-SS）")
        // 字符串表示
        .def("__repr__", [](const CmdParam& p) {
            return fmt::format("CmdParam(year={}, mon={}, day={}, hour={}, min={}, sec={}, ju={}, type={}, zone={}, "
                               "is_auto={}, str_dt='{}')",
                               p.year, p.mon, p.day, p.hour, p.min, p.sec, p.ju, p.type, p.zone, p.is_auto, p.str_dt);
        });

    // 绑定 CQimenUse 类
    py::class_<CQimenUse>(m, "CQimenUse", "奇门排盘核心逻辑类")
        .def(py::init<>(), "构造函数")
        .def("run", &CQimenUse::run,
             "执行奇门排盘计算\n"
             "参数:\n"
             "    param (CmdParam): 排盘参数\n"
             "返回值:\n"
             "    None",
             py::arg("param"));
}