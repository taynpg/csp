#include "print.h"

#include <iomanip>
#include <iostream>

#include "csp_base.hpp"
#include "zh_lang.h"

constexpr auto SPLIT_LINE = "==============================================";

QimenPrint::QimenPrint() = default;

QimenPrint::~QimenPrint() = default;

void QimenPrint::print_one(int g)
{
    const auto& qmd = qm_->get_result();
    if (g == 8) {
        std::cout << "              ";
        return;
    }
    std::string bashen = CZhData::bs(qmd.bashenr[g]);
    std::cout << ConsoleColor::BrightBlack << "  " << bashen << " ";
    if (g == qmd.maxing) {
        if (g == qmd.kongw[0] || g == qmd.kongw[1]) {
            std::cout << ConsoleColor::White << " (" << ConsoleColor::Green << CZhData::mx() + CZhData::kw()
                      << ConsoleColor::White << ")";
        } else {
            std::cout << ConsoleColor::White << " (" << ConsoleColor::Green << CZhData::mx() << ConsoleColor::White
                      << ")  ";
        }
    } else if (g == qmd.kongw[0] || g == qmd.kongw[1]) {
        std::cout << ConsoleColor::White << " (" << ConsoleColor::Green << CZhData::kw() << ConsoleColor::White
                  << ")  ";
    } else {
        std::cout << "       ";
    }
}

void QimenPrint::print_two(int g)
{
    const auto& qmd = qm_->get_result();
    if (g == 8) {
        std::cout << "              ";
        return;
    }
    std::string jiuxing = CZhData::jx(qmd.jiuxr[g]);
    std::cout << ConsoleColor::BrightGreen << "  " << jiuxing << "   ";
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
    std::cout << ConsoleColor::BrightYellow << szprint;
}

void QimenPrint::print_three(int g)
{
    const auto& qmd = qm_->get_result();
    if (g == 8) {
        std::cout << "              ";
        return;
    }
    std::string bamen = CZhData::bm(qmd.bamenr[g]);
    std::cout << ConsoleColor::Cyan << "  " << bamen << "   ";
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
    std::cout << ConsoleColor::White << szprint;
}

void QimenPrint::print_base()
{
    auto qmd = qm_->get_result();
    auto cal = qmd.dt_;
    const auto& solar = Qimen::solar(*cal);
    const auto& lunar = Qimen::lunar(*cal);

    std::cout << ConsoleColor::Green << SPLIT_LINE << std::endl;

    std::cout << "公元：" << ConsoleColor::White << std::setw(4) << std::setfill('0') << solar.year << "年";
    std::cout << ConsoleColor::White << std::setw(2) << std::setfill('0') << solar.mon << "月";
    std::cout << ConsoleColor::White << std::setw(2) << std::setfill('0') << solar.day << "日 ";
    std::cout << ConsoleColor::White << std::setw(2) << std::setfill('0') << solar.hour << "时";
    std::cout << ConsoleColor::White << std::setw(2) << std::setfill('0') << solar.min << "分";
    std::cout << ConsoleColor::White << std::setw(2) << std::setfill('0') << solar.sec << "秒       ";
    std::cout << ConsoleColor::Cyan << CSP_VERSION << std::endl;

    std::cout << ConsoleColor::Green << SPLIT_LINE << std::endl;

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
    std::cout << "农历：" << ConsoleColor::BrightGreen << yearToChinese(lunar.year) << "年 ";
    std::cout << ConsoleColor::BrightGreen << CZhData::mon(lunar.mon - 1) << "月";
    std::cout << ConsoleColor::BrightGreen << CZhData::lunar_day(lunar.day - 1) << "日 ";
    std::cout << ConsoleColor::BrightGreen << CZhData::zhi((lunar.hour + 1) / 2 % 12) << "时\n";

    // std::cout << ConsoleColor::Green << SPLIT_LINE  << std::endl;
    std::cout << ConsoleColor::Green << "干支：" << ConsoleColor::BrightGreen << CZhData::gan(gz.yi % 10) << "  ";
    std::cout << CZhData::gan(gz.mi % 10) << "  ";
    std::cout << CZhData::gan(gz.di % 10) << "  ";
    std::cout << CZhData::gan(gz.hi % 10) << "  \n";
    std::cout << "      " << CZhData::zhi(gz.yi % 12) << "  ";
    std::cout << CZhData::zhi(gz.mi % 12) << "  ";
    std::cout << CZhData::zhi(gz.di % 12) << "  ";
    std::cout << CZhData::zhi(gz.hi % 12) << "      ";

    switch (type_) {
    case 1: {
        std::cout << ConsoleColor::Green << "<时家转盘超接置润法>" << std::endl;
        break;
    }
    case 2: {
        std::string szYueJiang = CZhData::zhi(qmd.yuejiang_);
        std::cout << ConsoleColor::Green << " (月将:" << szYueJiang << ")(时家阴盘)" << std::endl;
        break;
    }
    case 3: {
        std::cout << ConsoleColor::Green << "    <时家转盘拆补法>" << std::endl;
        break;
    }
    case 4: {
        std::cout << ConsoleColor::Green << "    <时家转盘茅山法>" << std::endl;
        break;
    }
    default:
        break;
    }

    std::cout << ConsoleColor::Green << SPLIT_LINE << std::endl;

    std::cout << "值符：" << ConsoleColor::Cyan << CZhData::jx(qmd.jiuxp[qmd.duty]);
    std::cout << ConsoleColor::Green << "  值使：" << ConsoleColor::Cyan << CZhData::bm(qmd.bamenp[qmd.duty]) << " ";

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
    std::cout << ConsoleColor::White << "   [" << ConsoleColor::Cyan << szYuan << ConsoleColor::White << "]";
    std::string dun;
    if (qmd.is_yin) {
        dun = "阴遁" + CZhData::num(qmd.ju);
    } else {
        dun = "阳遁" + CZhData::num(qmd.ju);
    }
    std::cout << ConsoleColor::White << "[" << ConsoleColor::Cyan << dun << "局" << ConsoleColor::White << "]\n";
    std::cout << ConsoleColor::Green << SPLIT_LINE << std::endl;
}

void QimenPrint::print_other()
{
    auto qmd = qm_->get_result();
    std::cout << ConsoleColor::Green << "旬空：";
    std::string szXunkong;
    szXunkong += CZhData::zhi(qmd.xunkong[0]) + CZhData::zhi(qmd.xunkong[1]) + "  ";
    szXunkong += CZhData::zhi(qmd.xunkong[2]) + CZhData::zhi(qmd.xunkong[3]) + "  ";
    szXunkong += CZhData::zhi(qmd.xunkong[4]) + CZhData::zhi(qmd.xunkong[5]) + "  ";
    szXunkong += CZhData::zhi(qmd.xunkong[6]) + CZhData::zhi(qmd.xunkong[7]) + "  \n";
    std::cout << ConsoleColor::BrightGreen << szXunkong;
    std::cout << ConsoleColor::Green << SPLIT_LINE << std::endl;

    int firstIndex{};
    int secondIndex{};
    auto fq = Qimen::get_jq(*qmd.dt_, firstIndex, false);
    auto sq = Qimen::get_jq(*qmd.dt_, secondIndex, true);
    auto fqdt = Qimen::solar(fq);
    auto sqdt = Qimen::solar(sq);

    std::cout << ConsoleColor::White << "[" << ConsoleColor::Green << CZhData::jq(firstIndex) << ConsoleColor::White
              << "]：";
    char szTem[128]{};
    std::snprintf(szTem, sizeof(szTem), "%02d-%02d %02d:%02d:%02d", fqdt.mon, fqdt.day, fqdt.hour, fqdt.min, fqdt.sec);
    std::cout << ConsoleColor::BrightGreen << std::string(szTem);

    std::cout << ConsoleColor::White << "  [" << ConsoleColor::Green << CZhData::jq(secondIndex) << ConsoleColor::White
              << "]：";
    std::snprintf(szTem, sizeof(szTem), "%02d-%02d %02d:%02d:%02d", sqdt.mon, sqdt.day, sqdt.hour, sqdt.min, sqdt.sec);
    std::cout << ConsoleColor::BrightGreen << std::string(szTem) << std::endl;
    std::cout << ConsoleColor::Green << SPLIT_LINE << std::endl;
    std::cout << ConsoleColor::Green << "https://github.com/taynpg/csp" << std::endl;
    std::cout << ConsoleColor::Green << SPLIT_LINE << std::endl;
}

void QimenPrint::run(const std::shared_ptr<Qimen>& qm, int type)
{
    type_ = type;
    this->qm_ = qm;

    print_base();
    std::cout << ConsoleColor::Green << "|";
    print_one(4);
    std::cout << ConsoleColor::Green << "|";
    print_one(3);
    std::cout << ConsoleColor::Green << "|";
    print_one(2);
    std::cout << ConsoleColor::Green << "|\n";

    std::cout << ConsoleColor::Green << "|";
    print_two(4);
    std::cout << ConsoleColor::Green << "|";
    print_two(3);
    std::cout << ConsoleColor::Green << "|";
    print_two(2);
    std::cout << ConsoleColor::Green << "|\n";

    std::cout << ConsoleColor::Green << "|";
    print_three(4);
    std::cout << ConsoleColor::Green << "|";
    print_three(3);
    std::cout << ConsoleColor::Green << "|";
    print_three(2);
    std::cout << ConsoleColor::Green << "|\n";
    std::cout << ConsoleColor::Green << SPLIT_LINE << std::endl;

    std::cout << ConsoleColor::Green << "|";
    print_one(5);
    std::cout << ConsoleColor::Green << "|";
    print_one(8);
    std::cout << ConsoleColor::Green << "|";
    print_one(1);
    std::cout << ConsoleColor::Green << "|\n";

    std::cout << ConsoleColor::Green << "|";
    print_two(5);
    std::cout << ConsoleColor::Green << "|";
    print_two(8);
    std::cout << ConsoleColor::Green << "|";
    print_two(1);
    std::cout << ConsoleColor::Green << "|\n";

    std::cout << ConsoleColor::Green << "|";
    print_three(5);
    std::cout << ConsoleColor::Green << "|";
    print_three(8);
    std::cout << ConsoleColor::Green << "|";
    print_three(1);
    std::cout << ConsoleColor::Green << "|\n";
    std::cout << ConsoleColor::Green << SPLIT_LINE << std::endl;

    std::cout << ConsoleColor::Green << "|";
    print_one(6);
    std::cout << ConsoleColor::Green << "|";
    print_one(7);
    std::cout << ConsoleColor::Green << "|";
    print_one(0);
    std::cout << ConsoleColor::Green << "|\n";

    std::cout << ConsoleColor::Green << "|";
    print_two(6);
    std::cout << ConsoleColor::Green << "|";
    print_two(7);
    std::cout << ConsoleColor::Green << "|";
    print_two(0);
    std::cout << ConsoleColor::Green << "|\n";

    std::cout << ConsoleColor::Green << "|";
    print_three(6);
    std::cout << ConsoleColor::Green << "|";
    print_three(7);
    std::cout << ConsoleColor::Green << "|";
    print_three(0);
    std::cout << ConsoleColor::Green << "|\n";
    std::cout << ConsoleColor::Green << SPLIT_LINE << std::endl;
    print_other();
}
