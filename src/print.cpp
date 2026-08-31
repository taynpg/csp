#include "print.h"

#include <fmt/format.h>
#include <fmt/color.h>

#include "csp_base.hpp"
#include "zh_lang.h"

constexpr auto SPLIT_LINE = "=======================================================";

// 颜色样式别名
namespace {
    auto cc_bright_black  = fmt::fg(fmt::terminal_color::bright_black);
    auto cc_white         = fmt::fg(fmt::terminal_color::white);
    auto cc_green         = fmt::fg(fmt::terminal_color::green);
    auto cc_bright_green  = fmt::fg(fmt::terminal_color::bright_green);
    auto cc_bright_yellow = fmt::fg(fmt::terminal_color::bright_yellow);
    auto cc_cyan          = fmt::fg(fmt::terminal_color::cyan);
    auto cc_bright_white  = fmt::fg(fmt::terminal_color::bright_white);
}

QimenPrint::QimenPrint() = default;

QimenPrint::~QimenPrint() = default;

void QimenPrint::print_one(int g)
{
    const auto& qmd = qm_->get_result();
    if (g == 8) {
        fmt::print("                 ");
        return;
    }
    std::string bashen = CZhData::bs(qmd.bashenr[g]);
    fmt::print(cc_bright_black, "     {} ", bashen);
    if (g == qmd.maxing) {
        if (g == qmd.kongw[0] || g == qmd.kongw[1]) {
            fmt::print(cc_white, " (");
            fmt::print(cc_green, "{}", CZhData::mx() + CZhData::kw());
            fmt::print(cc_white, ")");
        } else {
            fmt::print(cc_white, " (");
            fmt::print(cc_green, "{}", CZhData::mx());
            fmt::print(cc_white, ")  ");
        }
    } else if (g == qmd.kongw[0] || g == qmd.kongw[1]) {
        fmt::print(cc_white, " (");
        fmt::print(cc_green, "{}", CZhData::kw());
        fmt::print(cc_white, ")  ");
    } else {
        fmt::print("       ");
    }
}

void QimenPrint::print_two(int g)
{
    const auto& qmd = qm_->get_result();
    if (g == 8) {
        fmt::print("                 ");
        return;
    }

    // 先打印暗干
    std::string angan = CZhData::gan(qmd.angan[g]);
    fmt::print(" {}", angan);

    std::string jiuxing = CZhData::jx(qmd.jiuxr[g]);
    fmt::print(cc_bright_green, "  {}   ", jiuxing);
    bool isJi = false;
    std::string szJi;
    // 如果此宫的星是寄宫星，那么就寄到这里
    if (qmd.jiuxr[g] == qmd.jiuxp[qmd.jigong]) {
        isJi = true;
        szJi = CZhData::gan(qmd.dp[8]);
    }
    std::string tian = CZhData::gan(qmd.tp[g]);
    std::string szprint = tian;
    if (isJi) {
        szprint += szJi + " ";
    } else {
        szprint += "   ";
    }
    fmt::print(cc_bright_yellow, "{}", szprint);
}

void QimenPrint::print_three(int g)
{
    const auto& qmd = qm_->get_result();
    if (g == 8) {
        fmt::print("                 ");
        return;
    }
    std::string bamen = CZhData::bm(qmd.bamenr[g]);
    fmt::print(cc_cyan, "     {}   ", bamen);
    bool isJi = false;
    std::string szJi;
    // 如果此宫的星是寄宫星，那么就寄到这里
    if (g == qmd.jigong) {
        isJi = true;
        szJi = CZhData::gan(qmd.dp[8]);
    }
    std::string di = CZhData::gan(qmd.dp[g]);
    std::string szprint = di;
    if (isJi) {
        szprint += szJi + " ";
    } else {
        szprint += "   ";
    }
    fmt::print(cc_white, "{}", szprint);
}

void QimenPrint::print_base()
{
    auto qmd = qm_->get_result();
    auto cal = qmd.dt_;
    const auto& solar = Qimen::solar(*cal);
    const auto& lunar = Qimen::lunar(*cal);

    fmt::print(cc_green, "{}\n", SPLIT_LINE);

    fmt::print("公元：");
    fmt::print(cc_white, "{:0>4}年", solar.year);
    fmt::print(cc_white, "{:0>2}月", solar.mon);
    fmt::print(cc_white, "{:0>2}日 ", solar.day);
    fmt::print(cc_white, "{:0>2}时", solar.hour);
    fmt::print(cc_white, "{:0>2}分", solar.min);
    fmt::print(cc_white, "{:0>2}秒                ", solar.sec);
    fmt::print(cc_cyan, "{}\n", CSP_VERSION);

    fmt::print(cc_green, "{}\n", SPLIT_LINE);

    auto yearToChinese = [](int year) -> std::string {
        std::string chineseYear;
        std::string yearStr = std::to_string(year);

        for (char digit : yearStr) {
            int num = digit - '0';
            chineseYear += CZhData::num(num);
        }
        return chineseYear;
    };

    const auto& gz = Qimen::jiazi(*cal);
    fmt::print("农历：");
    fmt::print(cc_white, "{}年 ", yearToChinese(lunar.year));
    fmt::print(cc_white, "{}月", CZhData::mon(lunar.mon - 1));
    fmt::print(cc_white, "{}日 ", CZhData::lunar_day(lunar.day - 1));
    fmt::print(cc_bright_green, "{}", CZhData::zhi((lunar.hour + 1) / 2 % 12));
    fmt::print(cc_white, "时\n");

    // fmt::print(cc_green, "{}\n", SPLIT_LINE);
    fmt::print(cc_green, "干支：");
    fmt::print(cc_bright_green, "{}  ", CZhData::gan(gz.yi % 10));
    fmt::print(cc_bright_green, "{}  ", CZhData::gan(gz.mi % 10));
    fmt::print(cc_bright_green, "{}  ", CZhData::gan(gz.di % 10));
    fmt::print(cc_bright_green, "{}  \n", CZhData::gan(gz.hi % 10));
    fmt::print("      {}  ", CZhData::zhi(gz.yi % 12));
    fmt::print("{}  ", CZhData::zhi(gz.mi % 12));
    fmt::print("{}  ", CZhData::zhi(gz.di % 12));
    fmt::print("{}      ", CZhData::zhi(gz.hi % 12));

    switch (type_) {
    case 1: {
        fmt::print(cc_green, "         <时家转盘超接置润法>\n");
        break;
    }
    case 2: {
        std::string szYueJiang = CZhData::zhi(qmd.yuejiang_);
        fmt::print(cc_green, "          (月将:{})(时家阴盘)\n", szYueJiang);
        break;
    }
    case 3: {
        fmt::print(cc_green, "             <时家转盘拆补法>\n");
        break;
    }
    case 4: {
        fmt::print(cc_green, "             <时家转盘茅山法>\n");
        break;
    }
    default:
        break;
    }

    fmt::print(cc_green, "{}\n", SPLIT_LINE);

    fmt::print("值符：");
    fmt::print(cc_cyan, "{}", CZhData::jx(qmd.jiuxp[qmd.duty]));
    fmt::print(cc_green, "  值使：");
    fmt::print(cc_cyan, "{} ", CZhData::bm(qmd.bamenp[qmd.duty]));

    std::string szYuan;
    switch (qmd.yuan) {
    case -1: {
        szYuan = "自动定局";
        break;
    }
    case 0:
        szYuan = "手动定局";
        break;
    case 1:
        szYuan = "下元";
        break;
    case 2:
        szYuan = "中元";
        break;
    case 3:
        szYuan = "上元";
        break;
    default:
        break;
    }
    std::string jieQi = CZhData::jq(qmd.jieq);
    if (qmd.yuan == 0 || qmd.yuan == -1) {
        szYuan = szYuan;
    } else {
        szYuan = jieQi + szYuan;
    }
    fmt::print(cc_white, "            [");
    fmt::print(cc_bright_white, "{}", szYuan);
    fmt::print(cc_white, "]");
    std::string dun;
    if (qmd.is_yin) {
        dun = "阴遁" + CZhData::num(qmd.ju);
    } else {
        dun = "阳遁" + CZhData::num(qmd.ju);
    }
    fmt::print(cc_white, "[");
    fmt::print(cc_cyan, "{}局", dun);
    fmt::print(cc_white, "]\n");
    fmt::print(cc_green, "{}\n", SPLIT_LINE);
}

void QimenPrint::print_other()
{
    auto qmd = qm_->get_result();
    fmt::print(cc_green, "旬空：");
    std::string szXunkong;
    szXunkong += CZhData::zhi(qmd.xunkong[0]) + CZhData::zhi(qmd.xunkong[1]) + "  ";
    szXunkong += CZhData::zhi(qmd.xunkong[2]) + CZhData::zhi(qmd.xunkong[3]) + "  ";
    szXunkong += CZhData::zhi(qmd.xunkong[4]) + CZhData::zhi(qmd.xunkong[5]) + "  ";
    szXunkong += CZhData::zhi(qmd.xunkong[6]) + CZhData::zhi(qmd.xunkong[7]) + "  \n";
    fmt::print(cc_bright_green, "{}", szXunkong);
    fmt::print(cc_green, "{}\n", SPLIT_LINE);

    int firstIndex{};
    int secondIndex{};
    auto fq = Qimen::get_jq(*qmd.dt_, firstIndex, false);
    auto sq = Qimen::get_jq(*qmd.dt_, secondIndex, true);
    auto fqdt = Qimen::solar(fq);
    auto sqdt = Qimen::solar(sq);

    fmt::print(cc_white, "[");
    fmt::print(cc_green, "{}", CZhData::jq(firstIndex));
    fmt::print(cc_white, "]：");
    fmt::print(cc_bright_green, "{:0>2}-{:0>2} {:0>2}:{:0>2}:{:0>2}",
                 fqdt.mon, fqdt.day, fqdt.hour, fqdt.min, fqdt.sec);

    fmt::print(cc_white, "           [");
    fmt::print(cc_green, "{}", CZhData::jq(secondIndex));
    fmt::print(cc_white, "]：");
    fmt::print(cc_bright_green, "{:0>2}-{:0>2} {:0>2}:{:0>2}:{:0>2}\n",
                 sqdt.mon, sqdt.day, sqdt.hour, sqdt.min, sqdt.sec);
    fmt::print(cc_green, "{}\n", SPLIT_LINE);
    fmt::print(cc_green, "https://github.com/taynpg/csp\n");
    fmt::print(cc_green, "{}\n", SPLIT_LINE);
}

void QimenPrint::run(const std::shared_ptr<Qimen>& qm, int type)
{
    type_ = type;
    this->qm_ = qm;

    print_base();
    fmt::print(cc_green, "|");
    print_one(4);
    fmt::print(cc_green, "|");
    print_one(3);
    fmt::print(cc_green, "|");
    print_one(2);
    fmt::print(cc_green, "|\n");

    fmt::print(cc_green, "|");
    print_two(4);
    fmt::print(cc_green, "|");
    print_two(3);
    fmt::print(cc_green, "|");
    print_two(2);
    fmt::print(cc_green, "|\n");

    fmt::print(cc_green, "|");
    print_three(4);
    fmt::print(cc_green, "|");
    print_three(3);
    fmt::print(cc_green, "|");
    print_three(2);
    fmt::print(cc_green, "|\n");
    fmt::print(cc_green, "{}\n", SPLIT_LINE);

    fmt::print(cc_green, "|");
    print_one(5);
    fmt::print(cc_green, "|");
    print_one(8);
    fmt::print(cc_green, "|");
    print_one(1);
    fmt::print(cc_green, "|\n");

    fmt::print(cc_green, "|");
    print_two(5);
    fmt::print(cc_green, "|");
    print_two(8);
    fmt::print(cc_green, "|");
    print_two(1);
    fmt::print(cc_green, "|\n");

    fmt::print(cc_green, "|");
    print_three(5);
    fmt::print(cc_green, "|");
    print_three(8);
    fmt::print(cc_green, "|");
    print_three(1);
    fmt::print(cc_green, "|\n");
    fmt::print(cc_green, "{}\n", SPLIT_LINE);

    fmt::print(cc_green, "|");
    print_one(6);
    fmt::print(cc_green, "|");
    print_one(7);
    fmt::print(cc_green, "|");
    print_one(0);
    fmt::print(cc_green, "|\n");

    fmt::print(cc_green, "|");
    print_two(6);
    fmt::print(cc_green, "|");
    print_two(7);
    fmt::print(cc_green, "|");
    print_two(0);
    fmt::print(cc_green, "|\n");

    fmt::print(cc_green, "|");
    print_three(6);
    fmt::print(cc_green, "|");
    print_three(7);
    fmt::print(cc_green, "|");
    print_three(0);
    fmt::print(cc_green, "|\n");
    fmt::print(cc_green, "{}\n", SPLIT_LINE);
    print_other();
}
