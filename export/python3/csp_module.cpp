#include "../../src/csp_base.hpp"
#include "../../src/qmuse.h"
#include "pybind11/pybind11.h"
#include <fmt/format.h>
#include <iostream>
#include <pybind11/stl.h>

namespace py = pybind11;

/*

    这里的导出说明注释，之所以没有使用中文是因为 csp 的编码在 Windows 上是 GBK 编码。
    为什么不用 UTF-8 ，一是为了兼容 Windows XP 系统，二是 UTF-8 编码时，自定义的彩色
    打印将会失效（这个自定义彩色打印不仅仅是为了支持XP，它还比默认ANSI彩色打印颜色更丰富）。

    经过测试，Python 在导入模块时，不接受 GBK 编码，否则会报错无法导入。
    但是导入成功之后，是不影响 cpp 内部的 GBK 打印输出的，因此这里使用英文是避免 GBK 中文编码。

*/

PYBIND11_MODULE(csp_qm, m)
{
    // Module documentation
    m.doc() = "Python interface for Qimen Dunjia divination tool 'csp'";

    // Bind CmdParam struct
    py::class_<CmdParam>(m, "CmdParam", "Qimen parameter structure")
        .def(py::init<>(), "Default constructor")
        // Member variables
        .def_readwrite("year", &CmdParam::year, "Year (e.g., 2023)")
        .def_readwrite("mon", &CmdParam::mon, "Month (1-12)")
        .def_readwrite("day", &CmdParam::day, "Day (1-31)")
        .def_readwrite("hour", &CmdParam::hour, "Hour (0-23)")
        .def_readwrite("min", &CmdParam::min, "Minute (0-59)")
        .def_readwrite("sec", &CmdParam::sec, "Second (0-59)")
        .def_readwrite("ju", &CmdParam::ju, "Ju number (ignored when auto-calculating)")
        .def_readwrite("type", &CmdParam::type,
                       "Layout type (required):\n"
                       "1=Hour-based ZhiRun\n"
                       "2=Hour-based YinPan\n"
                       "3=Hour-based ChaiBu\n"
                       "4=Hour-based MaoShan")
        .def_readwrite("zone", &CmdParam::zone, "Timezone offset (hours, default 0)")
        .def_readwrite("is_auto", &CmdParam::is_auto, "Auto-calculate time (true/false)")
        .def_readwrite("str_dt", &CmdParam::str_dt, "Manual date string (format: YYYY-MM-DD-HH-MM-SS)")
        // String representation
        .def("__repr__", [](const CmdParam& p) {
            return fmt::format("CmdParam(year={}, mon={}, day={}, hour={}, min={}, sec={}, ju={}, type={}, zone={}, "
                               "is_auto={}, str_dt='{}')",
                               p.year, p.mon, p.day, p.hour, p.min, p.sec, p.ju, p.type, p.zone, p.is_auto, p.str_dt);
        });

    // Bind CQimenUse class
    py::class_<CQimenUse>(m, "CQimenUse", "Qimen layout calculation core class")
        .def(py::init<>(), "Constructor")
        .def("run", &CQimenUse::run,
             "Execute Qimen layout calculation\n"
             "Args:\n"
             "    param (CmdParam): Calculation parameters\n"
             "Returns:\n"
             "    None",
             py::arg("param"));
}