// csp - 奇门遁甲排盘库
// 应用层：命令行参数、排盘流程与渲染。
#ifndef CSP_APP_HPP
#define CSP_APP_HPP

#include <optional>
#include <string>

#include "csp/chart.hpp"
#include "csp/types.hpp"

namespace csp {

/// 一次排盘的输入参数
struct Options {
    DateTime time{};                                                  ///< 排盘时间（auto_time 为 false 时使用）
    QimenType type{QimenType::ZhiRun};                                ///< 盘式
    int ju{};                                                         ///< 局数，0 表示自动
    int zone_hours{};                                                 ///< 与本地时间的时差（小时）
    HiddenStemStyle hidden_stem{HiddenStemStyle::ByOriginalPalace};   ///< 暗干排法
    bool auto_time{false};                                            ///< 使用当前时间
    bool export_json{false};                                          ///< 导出 JSON 文件
    bool color{true};                                                 ///< 终端输出是否着色
    std::string date_text{};                                          ///< 日期原文，仅用于回显
};

/// 解析 "YYYY-MM-DD-HH-MM-SS" 格式的日期
bool parse_datetime_text(const std::string& text, DateTime& out, std::string* error = nullptr);

/// 校验参数范围，并把 auto_time 的时间填充为当前时间
bool validate(Options& options, std::string* error = nullptr);

/// 排盘结果
struct Result {
    Chart chart;
    Options options;
};

/// 按参数排盘
bool build_chart(const Options& options, Result& result, std::string* error = nullptr);

/// 校验参数、应用时差偏移并排盘；局数会回填到 options
bool prepare_result(Options& options, Result& result, std::string* error = nullptr);

/// 渲染为终端文本（含 ANSI 颜色，color 为 false 时输出纯文本）
std::string render_text(const Chart& chart, QimenType type, bool color);

/// 渲染为 JSON
std::string render_json(const Result& result);

/// 导出 JSON 到当前工作目录，成功时返回文件名
bool write_json_file(const Result& result, std::string* file_name = nullptr, std::string* error = nullptr);

/// 排盘并输出：等价于命令行的一次执行
class Runner
{
public:
    /// 排盘、打印，必要时导出 JSON。参数会回填自动定局的结果。
    bool run(Options& options);

    /// 最近一次排盘的结果，未排盘时为 nullptr
    const Chart* chart() const
    {
        return result_.has_value() ? &result_->chart : nullptr;
    }

private:
    std::optional<Result> result_{};
};

}   // namespace csp

#endif   // CSP_APP_HPP
