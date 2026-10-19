#include <string>
#include <string_view>

#include <fmt/color.h>
#include <fmt/format.h>

#include "csp/app.hpp"
#include "csp/calendar.hpp"
#include "csp/version.hpp"
#include "csp/zh.hpp"

namespace csp {

namespace {

constexpr std::string_view kSplitLine = "=======================================================";

/// 控制台上中宫的留白宽度
constexpr int kCenterCellWidth = 17;

/// fmt 的终端颜色枚举值本身就是 ANSI 转义码（30-37 为暗色，90-97 为亮色）
int ansi_code(fmt::terminal_color color)
{
    return static_cast<int>(color);
}

/// 文本缓冲，负责着色与拼接，屏蔽终端转义细节
class Text
{
public:
    explicit Text(bool color) : color_(color)
    {
    }

    void add(std::string_view text)
    {
        buffer_.append(text);
    }

    void spaces(int count)
    {
        buffer_.append(static_cast<std::size_t>(count), ' ');
    }

    template <typename... Args> void addf(fmt::format_string<Args...> format, Args&&... args)
    {
        buffer_ += fmt::format(format, std::forward<Args>(args)...);
    }

    void paint(fmt::terminal_color color, std::string_view text)
    {
        if (color_) {
            buffer_ += "\x1b[" + std::to_string(ansi_code(color)) + "m";
        }
        buffer_.append(text);
        if (color_) {
            buffer_ += "\x1b[0m";
        }
    }

    template <typename... Args>
    void paintf(fmt::terminal_color color, fmt::format_string<Args...> format, Args&&... args)
    {
        paint(color, fmt::format(format, std::forward<Args>(args)...));
    }

    std::string take()
    {
        return std::move(buffer_);
    }

private:
    std::string buffer_;
    bool color_;
};

/// 公元年份 -> 中文数字（一九八六）
std::string chinese_year(int year)
{
    std::string result;
    for (const char digit : std::to_string(year)) {
        result += zh::numeral(digit - '0');
    }
    return result;
}

/// 三元标签：自动/手动定局直说，其余为「节气 + 元」
std::string yuan_label(const Chart& chart)
{
    const auto name = zh::yuan_name(chart.yuan);
    if (chart.yuan == Yuan::Auto || chart.yuan == Yuan::Manual) {
        return std::string(name);
    }
    return std::string(zh::solar_term(chart.solar_term)) + std::string(name);
}

class Renderer
{
public:
    Renderer(const Chart& chart, QimenType type, bool color) : out_(color), chart_(chart), type_(type)
    {
    }

    std::string run()
    {
        base();
        grid(4, 3, 2, &Renderer::deity_line);
        grid(4, 3, 2, &Renderer::star_line);
        grid(4, 3, 2, &Renderer::gate_line);
        out_.paint(fmt::terminal_color::green, fmt::format("{}\n", kSplitLine));

        grid(5, 8, 1, &Renderer::deity_line);
        grid(5, 8, 1, &Renderer::star_line);
        grid(5, 8, 1, &Renderer::gate_line);
        out_.paint(fmt::terminal_color::green, fmt::format("{}\n", kSplitLine));

        grid(6, 7, 0, &Renderer::deity_line);
        grid(6, 7, 0, &Renderer::star_line);
        grid(6, 7, 0, &Renderer::gate_line);
        out_.paint(fmt::terminal_color::green, fmt::format("{}\n", kSplitLine));

        footer();
        return out_.take();
    }

private:
    using Painter = void (Renderer::*)(int);

    void grid(int first, int second, int third, Painter painter)
    {
        const int palaces[3] = {first, second, third};
        for (int i = 0; i < 3; ++i) {
            out_.paint(fmt::terminal_color::green, "|");
            (this->*painter)(palaces[i]);
        }
        out_.paint(fmt::terminal_color::green, "|\n");
    }

    /// 八神一行
    void deity_line(int palace)
    {
        if (palace == kCenterPalace) {
            out_.spaces(kCenterCellWidth);
            return;
        }
        out_.paintf(fmt::terminal_color::bright_black, "     {} ", zh::deity(chart_.deities[palace]));

        const bool is_horse = (palace == chart_.horse_palace);
        const bool is_void = (palace == chart_.void_palace[0] || palace == chart_.void_palace[1]);
        if (is_horse && is_void) {
            out_.paint(fmt::terminal_color::white, " (");
            out_.paint(fmt::terminal_color::green, "马空");
            out_.paint(fmt::terminal_color::white, ")");
        } else if (is_horse) {
            out_.paint(fmt::terminal_color::white, " (");
            out_.paint(fmt::terminal_color::green, zh::horse_mark());
            out_.paint(fmt::terminal_color::white, ")  ");
        } else if (is_void) {
            out_.paint(fmt::terminal_color::white, " (");
            out_.paint(fmt::terminal_color::green, zh::void_mark());
            out_.paint(fmt::terminal_color::white, ")  ");
        } else {
            out_.add("       ");
        }
    }

    /// 暗干、九星、天盘一行
    void star_line(int palace)
    {
        if (palace == kCenterPalace) {
            out_.spaces(kCenterCellWidth);
            return;
        }
        out_.addf(" {}", zh::heaven_stem(chart_.hidden_stem[palace]));
        out_.paintf(fmt::terminal_color::bright_green, "  {}   ", zh::star(chart_.stars[palace]));

        std::string text(zh::heaven_stem(chart_.heaven_stem[palace]));
        if (chart_.stars[palace] == chart_.stars_origin[chart_.lodging_palace]) {
            // 此宫的星是寄宫星，天盘要带上寄宫的地盘干
            text += std::string(zh::heaven_stem(chart_.earth_stem[kCenterPalace])) + " ";
        } else {
            text += "   ";
        }
        out_.paint(fmt::terminal_color::bright_yellow, text);
    }

    /// 八门、地盘一行
    void gate_line(int palace)
    {
        if (palace == kCenterPalace) {
            out_.spaces(kCenterCellWidth);
            return;
        }
        out_.paintf(fmt::terminal_color::cyan, "     {}   ", zh::gate(chart_.gates[palace]));

        std::string text(zh::heaven_stem(chart_.earth_stem[palace]));
        if (palace == chart_.lodging_palace) {
            text += std::string(zh::heaven_stem(chart_.earth_stem[kCenterPalace])) + " ";
        } else {
            text += "   ";
        }
        out_.paint(fmt::terminal_color::white, text);
    }

    /// 抬头信息
    void base()
    {
        const auto& time = *chart_.time;
        const auto solar = cal::from_solar(time);
        const auto lunar = cal::from_lunar(time);
        const auto pillars = cal::four_pillars(time);

        out_.paint(fmt::terminal_color::green, fmt::format("{}\n", kSplitLine));

        out_.add("公元：");
        out_.paintf(fmt::terminal_color::white, "{:0>4}年", solar.year);
        out_.paintf(fmt::terminal_color::white, "{:0>2}月", solar.mon);
        out_.paintf(fmt::terminal_color::white, "{:0>2}日 ", solar.day);
        out_.paintf(fmt::terminal_color::white, "{:0>2}时", solar.hour);
        out_.paintf(fmt::terminal_color::white, "{:0>2}分", solar.min);
        out_.paintf(fmt::terminal_color::white, "{:0>2}秒                ", solar.sec);
        out_.paintf(fmt::terminal_color::cyan, "{}\n", kVersion);
        out_.paint(fmt::terminal_color::green, fmt::format("{}\n", kSplitLine));

        out_.add("农历：");
        out_.paintf(fmt::terminal_color::white, "{}年 ", chinese_year(lunar.year));
        out_.paintf(fmt::terminal_color::white, "{}月", zh::month(lunar.mon - 1));
        out_.paintf(fmt::terminal_color::white, "{}日 ", zh::lunar_day(lunar.day - 1));
        out_.paint(fmt::terminal_color::bright_green, zh::earth_branch((lunar.hour + 1) / 2 % 12));
        out_.paint(fmt::terminal_color::white, "时\n");

        out_.paint(fmt::terminal_color::green, "干支：");
        out_.paintf(fmt::terminal_color::bright_green, "{}  ", zh::heaven_stem(pillars.year % 10));
        out_.paintf(fmt::terminal_color::bright_green, "{}  ", zh::heaven_stem(pillars.month % 10));
        out_.paintf(fmt::terminal_color::bright_green, "{}  ", zh::heaven_stem(pillars.day % 10));
        out_.paintf(fmt::terminal_color::bright_green, "{}  \n", zh::heaven_stem(pillars.hour % 10));
        out_.addf("      {}  ", zh::earth_branch(pillars.year % 12));
        out_.addf("{}  ", zh::earth_branch(pillars.month % 12));
        out_.addf("{}  ", zh::earth_branch(pillars.day % 12));
        out_.addf("{}      ", zh::earth_branch(pillars.hour % 12));

        switch (type_) {
        case QimenType::ZhiRun:
            out_.paint(fmt::terminal_color::green, "         <时家转盘超接置润法>\n");
            break;
        case QimenType::YinPan:
            out_.paintf(fmt::terminal_color::green, "          (月将:{})(时家阴盘)\n",
                        zh::earth_branch(chart_.moon_general));
            break;
        case QimenType::ChaiBu:
            out_.paint(fmt::terminal_color::green, "             <时家转盘拆补法>\n");
            break;
        case QimenType::MaoShan:
            out_.paint(fmt::terminal_color::green, "             <时家转盘茅山法>\n");
            break;
        }
        out_.paint(fmt::terminal_color::green, fmt::format("{}\n", kSplitLine));

        out_.add("值符：");
        out_.paint(fmt::terminal_color::cyan, zh::star(chart_.stars_origin[chart_.duty_palace]));
        out_.paint(fmt::terminal_color::green, "  值使：");
        out_.paintf(fmt::terminal_color::cyan, "{} ", zh::gate(chart_.gates_origin[chart_.duty_palace]));

        out_.paint(fmt::terminal_color::white, "            [");
        out_.paint(fmt::terminal_color::bright_white, yuan_label(chart_));
        out_.paint(fmt::terminal_color::white, "]");
        out_.paint(fmt::terminal_color::white, "[");
        out_.paintf(fmt::terminal_color::cyan, "{}{}局", chart_.yin_dun ? "阴遁" : "阳遁", zh::numeral(chart_.ju));
        out_.paint(fmt::terminal_color::white, "]\n");
        out_.paint(fmt::terminal_color::green, fmt::format("{}\n", kSplitLine));
    }

    /// 旬空、节气、页脚
    void footer()
    {
        out_.paint(fmt::terminal_color::green, "旬空：");
        std::string text;
        for (const auto& pair : chart_.void_branch) {
            text += zh::earth_branch(pair[0]);
            text += zh::earth_branch(pair[1]);
            text += "  ";
        }
        text += "\n";
        out_.paint(fmt::terminal_color::bright_green, text);
        out_.paint(fmt::terminal_color::green, fmt::format("{}\n", kSplitLine));

        const auto span = cal::terms_around(*chart_.time);
        const auto previous = cal::from_solar(span.previous.time);
        const auto next = cal::from_solar(span.next.time);

        out_.paint(fmt::terminal_color::white, "[");
        out_.paint(fmt::terminal_color::green, zh::solar_term(span.previous.index));
        out_.paint(fmt::terminal_color::white, "]：");
        out_.paintf(fmt::terminal_color::bright_green, "{:0>2}-{:0>2} {:0>2}:{:0>2}:{:0>2}", previous.mon, previous.day,
                    previous.hour, previous.min, previous.sec);

        out_.paint(fmt::terminal_color::white, "           [");
        out_.paint(fmt::terminal_color::green, zh::solar_term(span.next.index));
        out_.paint(fmt::terminal_color::white, "]：");
        out_.paintf(fmt::terminal_color::bright_green, "{:0>2}-{:0>2} {:0>2}:{:0>2}:{:0>2}\n", next.mon, next.day,
                    next.hour, next.min, next.sec);
        out_.paint(fmt::terminal_color::green, fmt::format("{}\n", kSplitLine));
        out_.paint(fmt::terminal_color::green, fmt::format("{}\n", kProjectUrl));
        out_.paint(fmt::terminal_color::green, fmt::format("{}\n", kSplitLine));
    }

    Text out_;
    const Chart& chart_;
    QimenType type_;
};

}   // namespace

std::string render_text(const Chart& chart, QimenType type, bool color)
{
    Renderer renderer(chart, type, color);
    return renderer.run();
}

}   // namespace csp
