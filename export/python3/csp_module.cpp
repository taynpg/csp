#include "../../src/csp_base.hpp"
#include "../../src/qmuse.h"
#include "pybind11/pybind11.h"
#include <cstdio>
#include <fmt/format.h>
#include <iostream>
#include <pybind11/stl.h>
#include <vector>


#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

namespace {
// 参考 main.cpp 的 set_output_supply：启用 ANSI 转义（颜色）代码支持
void set_output_supply()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (hConsole != INVALID_HANDLE_VALUE && GetConsoleMode(hConsole, &mode)) {
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hConsole, mode);
    }
}

HANDLE g_hOut = nullptr;
// 参考 main.cpp 的 u8ConsoleBuf：std::cout 的 UTF-8 控制台缓冲。
// 若标准输出被重定向（非控制台），则回退为直接写 stdout，避免输出丢失
class u8ConsoleBuf : public std::streambuf
{
protected:
    int overflow(int c = EOF) override
    {
        if (c != EOF) {
            char ch = static_cast<char>(c);
            xsputn(&ch, 1);
        }
        return c;
    }
    std::streamsize xsputn(const char* s, std::streamsize n) override
    {
        if (g_hOut == nullptr) {
            g_hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        }
        DWORD mode = 0;
        if (g_hOut != nullptr && g_hOut != INVALID_HANDLE_VALUE && GetConsoleMode(g_hOut, &mode)) {
            int wideLen = MultiByteToWideChar(CP_UTF8, 0, s, static_cast<int>(n), nullptr, 0);
            if (wideLen > 0) {
                std::vector<wchar_t> buffer(wideLen);
                MultiByteToWideChar(CP_UTF8, 0, s, static_cast<int>(n), buffer.data(), wideLen);
                DWORD written = 0;
                WriteConsoleW(g_hOut, buffer.data(), wideLen, &written, nullptr);
            }
        } else {
            fwrite(s, 1, static_cast<size_t>(n), stdout);
        }
        return n;
    }
};

u8ConsoleBuf g_u8Buf;
}   // namespace
#endif

// 初始化控制台输出支持（仅 Windows 生效），幂等可重复调用
void init_console()
{
#ifdef _WIN32
    std::cout.rdbuf(&g_u8Buf);
    set_output_supply();
#endif
}

namespace py = pybind11;

PYBIND11_MODULE(csp_qm, m)
{
    // 导入模块时自动初始化控制台支持（与 main.cpp 的 cmd() 中的处理保持一致）
    init_console();
    // 暴露给 Python，供特殊场景（如导入后控制台属性变化）手动重新初始化
    m.def("init_console", &init_console,
          "初始化控制台输出支持（启用ANSI颜色/UTF-8输出，仅Windows生效，导入模块时已自动调用）");

    // 模块文档
    m.doc() = "奇门遁甲排盘工具 csp 的 Python 接口";

    // 绑定 CmdParam 结构体
    py::class_<CmdParam>(m, "CmdParam", "奇门排盘参数结构体")
        .def(py::init<>(), "默认构造函数")
        // 成员变量
        .def_readwrite("year", &CmdParam::year, "年份（例如：2023）")
        .def_readwrite("mon", &CmdParam::mon, "月份（1-12）")
        .def_readwrite("day", &CmdParam::day, "日期（1-31）")
        .def_readwrite("hour", &CmdParam::hour, "小时（0-23）")
        .def_readwrite("min", &CmdParam::min, "分钟（0-59）")
        .def_readwrite("sec", &CmdParam::sec, "秒（0-59）")
        .def_readwrite("ju", &CmdParam::ju, "局数（默认自动定局，手动指定时生效）")
        .def_readwrite("type", &CmdParam::type,
                       "盘式类型（必填）：\n"
                       "1=时家转盘超接置润\n"
                       "2=时家转盘阴盘\n"
                       "3=时家转盘拆补\n"
                       "4=时家茅山")
        .def_readwrite("zone", &CmdParam::zone, "与现在的时间差值（单位为小时，默认为0）")
        .def_readwrite("angan", &CmdParam::angan,
                       "暗干排法（默认0）：\n"
                       "0=门原宫位地盘\n"
                       "1=入值使门")
        .def_readwrite("is_auto", &CmdParam::is_auto, "是否自动获取当前时间（true/false）")
        .def_readwrite("str_dt", &CmdParam::str_dt, "手动输入日期字符串（格式：YYYY-MM-DD-HH-MM-SS）")
        .def_readwrite("exportJson", &CmdParam::exportJson, "是否导出json文件（true/false）")
        // 字符串表示
        .def("__repr__", [](const CmdParam& p) {
            return fmt::format("CmdParam(year={}, mon={}, day={}, hour={}, min={}, sec={}, ju={}, type={}, zone={}, "
                               "angan={}, is_auto={}, str_dt='{}', exportJson={})",
                               p.year, p.mon, p.day, p.hour, p.min, p.sec, p.ju, p.type, p.zone, p.angan, p.is_auto,
                               p.str_dt, p.exportJson);
        });

    // 绑定 CQimenUse 类
    py::class_<CQimenUse>(m, "CQimenUse", "奇门排盘计算核心类")
        .def(py::init<>(), "构造函数")
        .def("run", &CQimenUse::run,
             "执行奇门排盘计算\n"
             "参数：\n"
             "    param (CmdParam)：排盘计算参数\n"
             "返回：\n"
             "    无",
             py::arg("param"));
}
