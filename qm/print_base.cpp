#include "print_base.h"

#include <iomanip>
#include <iostream>

#include "../csp_base.hpp"
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
    if (g == 8) {
        std::cout << "              ";
        return;
    }
    std::string bashen = CZhData::bs(qm_->get_bs()[g]);
    std::cout << COLOR_WHITE << "  " << bashen << " ";
    if (g == qm_->get_mx()) {
        if (g == qm_->get_kw()[0] || g == qm_->get_kw()[1]) {
            std::cout << COLOR_CYAN << " (" << CZhData::mx() + CZhData::kw() + ")" << COLOR_RESET;
        } else {
            std::cout << COLOR_CYAN << " (" << CZhData::mx() + ")  " << COLOR_RESET;
        }
    } else if (g == qm_->get_kw()[0] || g == qm_->get_kw()[1]) {
        std::cout << COLOR_CYAN << " (" << CZhData::kw() + ")  " << COLOR_RESET;
    } else {
        std::cout << "       ";
    }
}

void CCmdPrint::print_two(int g)
{
    if (g == 8) {
        std::cout << "              ";
        return;
    }
    std::string jiuxing = CZhData::jx(qm_->get_jx()[g]);
    std::cout << COLOR_GREEN << "  " << jiuxing << "   ";
    bool isJi = false;
    std::string szJi;
    // 如果此宫的星是寄宫星，那么就寄到这里
    if (qm_->get_jx()[g] == qm_->get_jx_pre()[qm_->get_jg()]) {
        isJi = true;
        szJi = CZhData::gan(qm_->get_dp()[8]);
    }
    std::string tian = CZhData::gan(qm_->get_tp()[g]);
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
    if (g == 8) {
        std::cout << "              ";
        return;
    }
    std::string bamen = CZhData::bm(qm_->get_bm()[g]);
    std::cout << COLOR_CYAN << "  " << bamen << "   ";
    bool isJi = false;
    std::string szJi;
    // 如果此宫的星是寄宫星，那么就寄到这里
    if (g == qm_->get_jg()) {
        isJi = true;
        szJi = CZhData::gan(qm_->get_dp()[8]);
    }
    std::string di = CZhData::gan(qm_->get_dp()[g]);
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
    const CDateTime& solar = qm_->get_cal()->get_solar();
    const CDateTime& lunar = qm_->get_cal()->get_lunnar();
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::cout << "公元：" << COLOR_CYAN << std::setw(4) << std::setfill('0') << solar.date_.year_ << COLOR_RESET << "年";
    std::cout << COLOR_CYAN << std::setw(2) << std::setfill('0') << solar.date_.mon_ << COLOR_RESET << "月";
    std::cout << COLOR_CYAN << std::setw(2) << std::setfill('0') << solar.date_.day_ << COLOR_RESET << "日 ";
    std::cout << COLOR_CYAN << std::setw(2) << std::setfill('0') << solar.time_.h_ << COLOR_RESET << "时";
    std::cout << COLOR_CYAN << std::setw(2) << std::setfill('0') << solar.time_.m_ << COLOR_RESET << "分";
    std::cout << COLOR_CYAN << std::setw(2) << std::setfill('0') << solar.time_.s_ << COLOR_RESET << "秒       ";
    std::cout << COLOR_CYAN << CSP_VERSION << COLOR_RESET << std::endl;

    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::string yearStr{};
    int lyear{};
    if (lunar.date_.year_ < 0) {
        lyear = -lunar.date_.year_;
        yearStr.append("前");
    } else {
        lyear = lunar.date_.year_;
    }

    bool add = false;
    int oya = lyear / 1000;
    int oyb = lyear / 100 % 10;
    int oyc = lyear / 10 % 10;
    int oyd = lyear % 10;

    if (oya != 0) add = true;
    if (add) yearStr.append(CZhData::num(oya));
    if (oyb != 0) add = true;
    if (add) yearStr.append(CZhData::num(oyb));
    if (oyc != 0) add = true;
    if (add) yearStr.append(CZhData::num(oyc));
    if (oyd != 0) add = true;
    if (add) yearStr.append(CZhData::num(oyd));

    const CGanZhi& gz = qm_->get_cal()->get_sz();
    std::cout << "阴历：" << COLOR_CYAN << yearStr << COLOR_RESET << "年 ";
    std::cout << COLOR_CYAN << CZhData::mon(lunar.date_.mon_ - 1) << COLOR_RESET << "月";
    std::cout << COLOR_CYAN << CZhData::lunar_day(lunar.date_.day_ - 1) << COLOR_RESET << "日 ";
    std::cout << COLOR_CYAN << CZhData::zhi((lunar.time_.h_ + 1) / 2 % 12) << COLOR_RESET << "时\n";

    // std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
    std::cout << "干支：" << COLOR_YELLOW << CZhData::gan(gz.yg_) << "  ";
    std::cout << CZhData::gan(gz.mg_) << "  ";
    std::cout << CZhData::gan(gz.dg_) << "  ";
    std::cout << CZhData::gan(gz.hg_) << "  \n";
    std::cout << "      " << CZhData::zhi(gz.yz_) << "  ";
    std::cout << CZhData::zhi(gz.mz_) << "  ";
    std::cout << CZhData::zhi(gz.dz_) << "  ";
    std::cout << CZhData::zhi(gz.hz_) << "      " << COLOR_RESET;

    switch (type_) {
        case 1: {
            std::cout << COLOR_CYAN << "<时家转盘超接置润法>" << COLOR_RESET << std::endl;
            break;
        }
        case 2: {
            std::string szYueJiang = CZhData::zhi(12 - qm_->get_cal()->get_lunnar().date_.mon_);
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

    std::cout << "值符：" << COLOR_YELLOW << CZhData::jx(qm_->get_jx_pre()[qm_->get_duty()]) << COLOR_RESET;
    std::cout << "  值使：" << COLOR_YELLOW << CZhData::bm(qm_->get_bm_pre()[qm_->get_duty()]) << " ";

    std::string szYuan;
    switch (qm_->get_yuan()) {
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
    std::string jieQi = CZhData::jq(qm_->get_jq());
    if (qm_->get_yuan() == 0) {
        szYuan = "   [" + szYuan + "]";
    } else {
        szYuan = "   [" + jieQi + szYuan + "]";
    }
    std::cout << COLOR_YELLOW << szYuan;

    std::string dun;
    if (qm_->is_yin()) {
        dun = "[阴遁" + CZhData::num(qm_->get_js()) + "局]\n";
    } else {
        dun = "[阳遁" + CZhData::num(qm_->get_js()) + "局]\n";
    }
    std::cout << COLOR_YELLOW << dun << COLOR_RESET;
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
}

void CCmdPrint::print_other()
{
    std::cout << "旬空：";
    std::string szXunkong;
    szXunkong += CZhData::zhi(qm_->get_xk()[0]) + CZhData::zhi(qm_->get_xk()[1]) + "   ";
    szXunkong += CZhData::zhi(qm_->get_xk()[2]) + CZhData::zhi(qm_->get_xk()[3]) + "   ";
    szXunkong += CZhData::zhi(qm_->get_xk()[4]) + CZhData::zhi(qm_->get_xk()[5]) + "   ";
    szXunkong += CZhData::zhi(qm_->get_xk()[6]) + CZhData::zhi(qm_->get_xk()[7]) + "   \n";
    std::cout << COLOR_YELLOW << szXunkong << COLOR_RESET;
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::string szJieName;
    const auto& first_jq = qm_->get_cal()->get_jie().jq[2];
    const auto& second_jq = qm_->get_cal()->get_jie().jq[3];
    szJieName += "[" + CZhData::jq(first_jq.index_) + "]：";
    std::cout << szJieName;
    char szTem[128]{};
    std::snprintf(szTem, sizeof(szTem), "%02d-%02d %02d:%02d:%02d", first_jq.dt_.date_.mon_, first_jq.dt_.date_.day_, first_jq.dt_.time_.h_,
                  first_jq.dt_.time_.m_, first_jq.dt_.time_.s_);
    std::cout << COLOR_YELLOW << std::string(szTem) << COLOR_RESET;
    szJieName.clear();
    szJieName += "  [" + CZhData::jq(second_jq.index_) + "]：";
    std::cout << szJieName;
    std::snprintf(szTem, sizeof(szTem), "%02d-%02d %02d:%02d:%02d", second_jq.dt_.date_.mon_, second_jq.dt_.date_.day_, second_jq.dt_.time_.h_,
                  second_jq.dt_.time_.m_, second_jq.dt_.time_.s_);
    std::cout << COLOR_YELLOW << std::string(szTem) << COLOR_RESET << std::endl;
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
    std::cout << COLOR_YELLOW << "https://github.com/taynpg/csp" << COLOR_RESET << std::endl;
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
}

void CCmdPrint::run(cppbox::CQimen* qm, int nType)
{
    type_ = nType;
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
