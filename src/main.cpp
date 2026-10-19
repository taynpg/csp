// csp - 奇门遁甲排盘工具，命令行入口
#include <cstdio>
#include <sstream>
#include <string>

#include <CLI11.hpp>
#include <fmt/format.h>

#include "csp/app.hpp"
#include "csp/console.hpp"
#include "csp/version.hpp"

int main(int argc, char** argv)
{
    csp::console::init();
    // init() 在 Windows 下会把 std::cout / std::cerr 换成 UTF-8 适配的 streambuf：
    // 控制台里转成宽字符写出，重定向到文件或管道时原样写字节。
    // 类 Unix 系统不需要这一步（UTF-8 字节本身就是终端要的）。

    csp::Options options;
    std::string date_text;
    int type = -1;
    int angan = 0;
    bool no_color = false;

    CLI::App app{fmt::format("命令行排盘工具 csp {} => {}", csp::kVersion, csp::kProjectUrl)};
    app.set_help_flag("-h,--help", "显示帮助并退出");
    app.set_version_flag("-v,--version", csp::kVersion, "显示版本并退出");
    // CLI11 自带的报错是英文的，这里补一层中文提示
    app.failure_message([](const CLI::App*, const CLI::Error& e) {
        return fmt::format("参数错误：{}\n使用 -h 或 --help 查看帮助。\n", e.what());
    });
    app.add_option("-t,--type", type,
                   "==> 盘式类型(必填) <==\n"
                   "[1,时家转盘超接置润]\n"
                   "[2,时家转盘阴盘]\n"
                   "[3,时家转盘拆补]\n"
                   "[4,时家茅山]")
        ->required()
        ->check(CLI::Range(1, 4));
    app.add_option("-d,--date", date_text,
                   "==> 输入日期(默认当前时间) <==\n"
                   "手动输入格式: 2000-2-3-15-32-11");
    app.add_option("--ju", options.ju, "局数(默认自动局数；-9 到 9，负数为阴遁)")->check(CLI::Range(-9, 9));
    app.add_option("-z,--zone", options.zone_hours, "与现在的时间差值(单位为小时，默认为0)")
        ->check(CLI::Range(-24, 24));
    app.add_option("-a,--angan", angan,
                   "暗干排法（默认0）：\n"
                   "[0,门原宫位地盘]\n"
                   "[1,入值使门]")
        ->check(CLI::Range(0, 1));
    app.add_flag("-j,--json", options.export_json, "导出json文件");
    app.add_flag("--no-color", no_color, "输出纯文本（便于重定向到文件）");

    // CLI11 默认把 UTF-8 字节直接写 std::cout，而 Windows 控制台按本地代码页（简体中文为 936）
    // 解释这些字节，帮助与错误信息会变成乱码。这里先把 CLI11 的输出收进字符串，
    // 再交给 console 层按 UTF-8 转宽字符输出。
    std::ostringstream brief;
    std::ostringstream detail;
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        const int code = app.exit(e, brief, detail);
        csp::console::write_error(detail.str());
        csp::console::write(brief.str());
        return code;
    }

    options.type = static_cast<csp::QimenType>(type);
    options.hidden_stem = static_cast<csp::HiddenStemStyle>(angan);
    // 重定向到文件或管道时不输出 ANSI 转义序列，否则存下来的文本会夹着乱码
    options.color = !no_color && csp::console::stdout_is_console();
    options.date_text = date_text;

    if (date_text.empty()) {
        options.auto_time = true;
    } else {
        std::string error;
        if (!csp::parse_datetime_text(date_text, options.time, &error)) {
            csp::console::write_error(fmt::format("{}\n", error));
            return 1;
        }
    }

    csp::Runner runner;
    return runner.run(options) ? 0 : 1;
}
