#include "print.h"

#include <iomanip>
#include <iostream>

#include "csp_base.hpp"
#include "zh_lang.h"

constexpr auto SPLIT_LINE = "==============================================";

CCmdPrint::CCmdPrint() = default;

CCmdPrint::~CCmdPrint() = default;

void CCmdPrint::print_one(int g)
{
    const auto& qmd = qm_->get_result();
    if (g == 8) {
        std::cout << "              ";
        return;
    }
    std::string bashen = CZhData::bs(qmd.bashenr[g]);
    std::cout << ConsoleColor::White << "  " << bashen << " ";
    if (g == qmd.maxing) {
        if (g == qmd.kongw[0] || g == qmd.kongw[1]) {
            std::cout << ConsoleColor::Cyan << " (" << CZhData::mx() + CZhData::kw() + ")";
        } else {
            std::cout << ConsoleColor::Cyan << " (" << CZhData::mx() + ")  ";
        }
    } else if (g == qmd.kongw[0] || g == qmd.kongw[1]) {
        std::cout << ConsoleColor::Cyan << " (" << CZhData::kw() + ")  ";
    } else {
        std::cout << "       ";
    }
}

void CCmdPrint::print_two(int g)
{
    const auto& qmd = qm_->get_result();
    if (g == 8) {
        std::cout << "              ";
        return;
    }
    std::string jiuxing = CZhData::jx(qmd.jiuxr[g]);
    std::cout << ConsoleColor::Green << "  " << jiuxing << "   ";
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
    std::cout << ConsoleColor::YellowIntensity << szprint;
}

void CCmdPrint::print_three(int g)
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
    std::cout << ConsoleColor::Yellow << szprint;
}

void CCmdPrint::print_base()
{
    auto qmd = qm_->get_result();
    auto cal = qmd.dt_;
    const auto& solar = Qimen::solar(*cal);
    const auto& lunar = Qimen::lunar(*cal);

    std::cout << ConsoleColor::Green << SPLIT_LINE << std::endl;

    std::cout << "公元：" << ConsoleColor::Cyan << std::setw(4) << std::setfill('0') << solar.year << "年";
    std::cout << ConsoleColor::Cyan << std::setw(2) << std::setfill('0') << solar.mon << "月";
    std::cout << ConsoleColor::Cyan << std::setw(2) << std::setfill('0') << solar.day << "日 ";
    std::cout << ConsoleColor::Cyan << std::setw(2) << std::setfill('0') << solar.hour << "时";
    std::cout << ConsoleColor::Cyan << std::setw(2) << std::setfill('0') << solar.min << "分";
    std::cout << ConsoleColor::Cyan << std::setw(2) << std::setfill('0') << solar.sec << "秒       ";
    std::cout << ConsoleColor::Cyan << CSP_VERSION << std::endl;

    std::cout << ConsoleColor::Green << SPLIT_LINE << std::endl;

    std::string yearStr{};
    int lyear{};
    if (lunar.year < 0) {
        lyear = -lunar.year;
        yearStr.append("前");
    } else {
        lyear = lunar.year;
    }

    bool add = false;
    int oya = lyear / 1000;
    int oyb = lyear / 100 % 10;
    int oyc = lyear / 10 % 10;
    int oyd = lyear % 10;

    if (oya != 0)
        add = true;
    if (add)
        yearStr.append(CZhData::num(oya));
    if (oyb != 0)
        add = true;
    if (add)
        yearStr.append(CZhData::num(oyb));
    if (oyc != 0)
        add = true;
    if (add)
        yearStr.append(CZhData::num(oyc));
    if (oyd != 0)
        add = true;
    if (add)
        yearStr.append(CZhData::num(oyd));

    const auto& gz = Qimen::jiazi(*cal);
    std::cout << "农历：" << ConsoleColor::Cyan << yearStr << "年 ";
    std::cout << ConsoleColor::Cyan << CZhData::mon(lunar.mon - 1) << "月";
    std::cout << ConsoleColor::Cyan << CZhData::lunar_day(lunar.day - 1) << "日 ";
    std::cout << ConsoleColor::Cyan << CZhData::zhi((lunar.hour + 1) / 2 % 12) << "时\n";

    // std::cout << ConsoleColor::Green << SPLIT_LINE  << std::endl;
    std::cout << "干支：" << ConsoleColor::Yellow << CZhData::gan(gz.yi % 10) << "  ";
    std::cout << CZhData::gan(gz.mi % 10) << "  ";
    std::cout << CZhData::gan(gz.di % 10) << "  ";
    std::cout << CZhData::gan(gz.hi % 10) << "  \n";
    std::cout << "      " << CZhData::zhi(gz.yi % 12) << "  ";
    std::cout << CZhData::zhi(gz.mi % 12) << "  ";
    std::cout << CZhData::zhi(gz.di % 12) << "  ";
    std::cout << CZhData::zhi(gz.hi % 12) << "      ";

    switch (type_) {
    case 1: {
        std::cout << ConsoleColor::Cyan << "<时家转盘超接置润法>" << std::endl;
        break;
    }
    case 2: {
        std::string szYueJiang = CZhData::zhi(qmd.yuejiang_);
        std::cout << ConsoleColor::Cyan << " (月将:" << szYueJiang << ")(时家阴盘)" << std::endl;
        break;
    }
    case 3: {
        std::cout << ConsoleColor::Cyan << "    <时家转盘拆补法>" << std::endl;
        break;
    }
    case 4: {
        std::cout << ConsoleColor::Cyan << "    <时家转盘茅山法>" << std::endl;
        break;
    }
    default:
        break;
    }

    std::cout << ConsoleColor::Green << SPLIT_LINE << std::endl;

    std::cout << "值符：" << ConsoleColor::Yellow << CZhData::jx(qmd.jiuxp[qmd.duty]);
    std::cout << "  值使：" << ConsoleColor::Yellow << CZhData::bm(qmd.bamenp[qmd.duty]) << " ";

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
        szYuan = "   [" + szYuan + "]";
    } else {
        szYuan = "   [" + jieQi + szYuan + "]";
    }
    std::cout << ConsoleColor::Yellow << szYuan;

    std::string dun;
    if (qmd.is_yin) {
        dun = "[阴遁" + CZhData::num(qmd.ju) + "局]\n";
    } else {
        dun = "[阳遁" + CZhData::num(qmd.ju) + "局]\n";
    }
    std::cout << ConsoleColor::Yellow << dun;
    std::cout << ConsoleColor::Green << SPLIT_LINE << std::endl;
}

void CCmdPrint::print_other()
{
    auto qmd = qm_->get_result();
    std::cout << "旬空：";
    std::string szXunkong;
    szXunkong += CZhData::zhi(qmd.xunkong[0]) + CZhData::zhi(qmd.xunkong[1]) + "   ";
    szXunkong += CZhData::zhi(qmd.xunkong[2]) + CZhData::zhi(qmd.xunkong[3]) + "   ";
    szXunkong += CZhData::zhi(qmd.xunkong[4]) + CZhData::zhi(qmd.xunkong[5]) + "   ";
    szXunkong += CZhData::zhi(qmd.xunkong[6]) + CZhData::zhi(qmd.xunkong[7]) + "   \n";
    std::cout << ConsoleColor::Yellow << szXunkong;
    std::cout << ConsoleColor::Green << SPLIT_LINE << std::endl;

    // std::string szJieName;
    // const auto& first_jq = qm_->get_cal()->get_jq_info()[2];
    // const auto& second_jq = qm_->get_cal()->get_jq_info()[3];
    // szJieName += "[" + CZhData::jq(first_jq.index) + "]：";
    // std::cout << szJieName;
    // char szTem[128]{};
    // std::snprintf(szTem, sizeof(szTem), "%02d-%02d %02d:%02d:%02d", first_jq.dt.mon, first_jq.dt.day,
    // first_jq.dt.hour,
    //               first_jq.dt.min, first_jq.dt.sec);
    // std::cout << ConsoleColor::Yellow << std::string(szTem);
    // szJieName.clear();
    // szJieName += "  [" + CZhData::jq(second_jq.index) + "]：";
    // std::cout << szJieName;
    // std::snprintf(szTem, sizeof(szTem), "%02d-%02d %02d:%02d:%02d", second_jq.dt.mon, second_jq.dt.day,
    //               second_jq.dt.hour, second_jq.dt.min, second_jq.dt.sec);
    // std::cout << ConsoleColor::Yellow << std::string(szTem) << std::endl;
    std::cout << ConsoleColor::Green << SPLIT_LINE << std::endl;
    std::cout << ConsoleColor::Yellow << "https://github.com/taynpg/csp" << std::endl;
    std::cout << ConsoleColor::Green << SPLIT_LINE << std::endl;
}

void CCmdPrint::run(const std::shared_ptr<Qimen>& qm, int type)
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
