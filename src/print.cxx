#include "print.h"

#include <iomanip>
#include <iostream>

#include "csp_base.hpp"
#include "zh_lang.h"

// 定义ANSI颜色码
#define COLOR_RESET "\033[0m"
#define COLOR_BLACK "\033[30m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"

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
    std::cout << COLOR_WHITE << "  " << bashen << " ";
    if (g == qmd.maxing) {
        if (g == qmd.kongw[0] || g == qmd.kongw[1]) {
            std::cout << COLOR_CYAN << " (" << CZhData::mx() + CZhData::kw() + ")" << COLOR_RESET;
        } else {
            std::cout << COLOR_CYAN << " (" << CZhData::mx() + ")  " << COLOR_RESET;
        }
    } else if (g == qmd.kongw[0] || g == qmd.kongw[1]) {
        std::cout << COLOR_CYAN << " (" << CZhData::kw() + ")  " << COLOR_RESET;
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
    std::cout << COLOR_GREEN << "  " << jiuxing << "   ";
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
    std::cout << COLOR_YELLOW << szprint << COLOR_RESET;
}

void CCmdPrint::print_three(int g)
{
    const auto& qmd = qm_->get_result();
    if (g == 8) {
        std::cout << "              ";
        return;
    }
    std::string bamen = CZhData::bm(qmd.bamenr[g]);
    std::cout << COLOR_CYAN << "  " << bamen << "   ";
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
    std::cout << COLOR_YELLOW << szprint << COLOR_RESET;
}

void CCmdPrint::print_base()
{
    auto qmd = qm_->get_result();
    auto cal = qm_->get_cal();
    const auto& solar = qm_->get_cal()->get_solar();
    const auto& lunar = qm_->get_cal()->get_lunar();

    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::cout << "公元：" << COLOR_CYAN << std::setw(4) << std::setfill('0') << solar.year << COLOR_RESET << "年";
    std::cout << COLOR_CYAN << std::setw(2) << std::setfill('0') << solar.mon << COLOR_RESET << "月";
    std::cout << COLOR_CYAN << std::setw(2) << std::setfill('0') << solar.day << COLOR_RESET << "日 ";
    std::cout << COLOR_CYAN << std::setw(2) << std::setfill('0') << solar.hour << COLOR_RESET << "时";
    std::cout << COLOR_CYAN << std::setw(2) << std::setfill('0') << solar.min << COLOR_RESET << "分";
    std::cout << COLOR_CYAN << std::setw(2) << std::setfill('0') << solar.sec << COLOR_RESET << "秒       ";
    std::cout << COLOR_CYAN << CSP_VERSION << COLOR_RESET << std::endl;

    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

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

    const auto& gz = cal->get_ganzhi();
    std::cout << "阴历：" << COLOR_CYAN << yearStr << COLOR_RESET << "年 ";
    std::cout << COLOR_CYAN << CZhData::mon(lunar.mon - 1) << COLOR_RESET << "月";
    std::cout << COLOR_CYAN << CZhData::lunar_day(lunar.day - 1) << COLOR_RESET << "日 ";
    std::cout << COLOR_CYAN << CZhData::zhi((lunar.hour + 1) / 2 % 12) << COLOR_RESET << "时\n";

    // std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
    std::cout << "干支：" << COLOR_YELLOW << CZhData::gan(gz.yg) << "  ";
    std::cout << CZhData::gan(gz.mg) << "  ";
    std::cout << CZhData::gan(gz.dg) << "  ";
    std::cout << CZhData::gan(gz.hg) << "  \n";
    std::cout << "      " << CZhData::zhi(gz.yz) << "  ";
    std::cout << CZhData::zhi(gz.mz) << "  ";
    std::cout << CZhData::zhi(gz.dz) << "  ";
    std::cout << CZhData::zhi(gz.hz) << "      " << COLOR_RESET;

    switch (type_) {
    case 1: {
        std::cout << COLOR_CYAN << "<时家转盘超接置润法>" << COLOR_RESET << std::endl;
        break;
    }
    case 2: {
        std::string szYueJiang = CZhData::zhi(12 - cal->get_lunar().mon);
        std::cout << COLOR_CYAN << " (月将:" << szYueJiang << ")(时家阴盘)" << COLOR_RESET << std::endl;
        break;
    }
    case 3: {
        std::cout << COLOR_CYAN << "    <时家转盘拆补法>" << COLOR_RESET << std::endl;
        break;
    }
    case 4: {
        std::cout << COLOR_CYAN << "    <时家转盘茅山法>" << COLOR_RESET << std::endl;
        break;
    }
    default:
        break;
    }

    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::cout << "值符：" << COLOR_YELLOW << CZhData::jx(qmd.jiuxp[qmd.duty]) << COLOR_RESET;
    std::cout << "  值使：" << COLOR_YELLOW << CZhData::bm(qmd.bamenp[qmd.duty]) << " ";

    std::string szYuan;
    switch (qmd.yuan) {
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
    if (qmd.yuan == 0) {
        szYuan = "   [" + szYuan + "]";
    } else {
        szYuan = "   [" + jieQi + szYuan + "]";
    }
    std::cout << COLOR_YELLOW << szYuan;

    std::string dun;
    if (qmd.is_yin) {
        dun = "[阴遁" + CZhData::num(qmd.ju) + "局]\n";
    } else {
        dun = "[阳遁" + CZhData::num(qmd.ju) + "局]\n";
    }
    std::cout << COLOR_YELLOW << dun << COLOR_RESET;
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
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
    std::cout << COLOR_YELLOW << szXunkong << COLOR_RESET;
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::string szJieName;
    const auto& first_jq = qm_->get_cal()->get_jq_info()[2];
    const auto& second_jq = qm_->get_cal()->get_jq_info()[3];
    szJieName += "[" + CZhData::jq(first_jq.index) + "]：";
    std::cout << szJieName;
    char szTem[128]{};
    std::snprintf(szTem, sizeof(szTem), "%02d-%02d %02d:%02d:%02d", first_jq.dt.mon, first_jq.dt.day, first_jq.dt.hour,
                  first_jq.dt.min, first_jq.dt.sec);
    std::cout << COLOR_YELLOW << std::string(szTem) << COLOR_RESET;
    szJieName.clear();
    szJieName += "  [" + CZhData::jq(second_jq.index) + "]：";
    std::cout << szJieName;
    std::snprintf(szTem, sizeof(szTem), "%02d-%02d %02d:%02d:%02d", second_jq.dt.mon, second_jq.dt.day, second_jq.dt.hour,
                  second_jq.dt.min, second_jq.dt.sec);
    std::cout << COLOR_YELLOW << std::string(szTem) << COLOR_RESET << std::endl;
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
    std::cout << COLOR_YELLOW << "https://github.com/taynpg/csp" << COLOR_RESET << std::endl;
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
}

void CCmdPrint::run(const std::shared_ptr<Qimen>& qm, int type)
{
    type_ = type;
    this->qm_ = qm;

    print_base();
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_one(4);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_one(3);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_one(2);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;

    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_two(4);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_two(3);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_two(2);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;

    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_three(4);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_three(3);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_three(2);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_one(5);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_one(8);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_one(1);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;

    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_two(5);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_two(8);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_two(1);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;

    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_three(5);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_three(8);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_three(1);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_one(6);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_one(7);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_one(0);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;

    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_two(6);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_two(7);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_two(0);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;

    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_three(6);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_three(7);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    print_three(0);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
    print_other();
}
