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
        std::wcout << L"              ";
        return;
    }
    std::wstring bashen = CZhData::bs(qm_->get_bs()[g]);
    std::wcout << COLOR_WHITE << L"  " << bashen << L" ";
    if (g == qm_->get_mx()) {
        if (g == qm_->get_kw()[0] || g == qm_->get_kw()[1]) {
            std::wcout << COLOR_CYAN << L" (" << CZhData::mx() + CZhData::kw() + L")" << COLOR_RESET;
        } else {
            std::wcout << COLOR_CYAN << L" (" << CZhData::mx() + L")  " << COLOR_RESET;
        }
    } else if (g == qm_->get_kw()[0] || g == qm_->get_kw()[1]) {
        std::wcout << COLOR_CYAN << L" (" << CZhData::kw() + L")  " << COLOR_RESET;
    } else {
        std::wcout << L"       ";
    }
}

void CCmdPrint::print_two(int g)
{
    if (g == 8) {
        std::wcout << L"              ";
        return;
    }
    std::wstring jiuxing = CZhData::jx(qm_->get_jx()[g]);
    std::wcout << COLOR_GREEN << L"  " << jiuxing << L"   ";
    bool isJi = false;
    std::wstring szJi;
    // 如果此宫的星是寄宫星，那么就寄到这里
    if (qm_->get_jx()[g] == qm_->get_jx_pre()[qm_->get_jg()]) {
        isJi = true;
        szJi = CZhData::gan(qm_->get_dp()[8]);
    }
    std::wstring tian = CZhData::gan(qm_->get_tp()[g]);
    std::wstring szprint = tian;
    if (isJi) {
        szprint += szJi + L" ";
    } else {
        szprint += L"   ";
    }
    std::wcout << COLOR_YELLOW << szprint << COLOR_RESET;
}

void CCmdPrint::print_three(int g)
{
    if (g == 8) {
        std::wcout << L"              ";
        return;
    }
    std::wstring bamen = CZhData::bm(qm_->get_bm()[g]);
    std::wcout << COLOR_CYAN << L"  " << bamen << L"   ";
    bool isJi = false;
    std::wstring szJi;
    // 如果此宫的星是寄宫星，那么就寄到这里
    if (g == qm_->get_jg()) {
        isJi = true;
        szJi = CZhData::gan(qm_->get_dp()[8]);
    }
    std::wstring di = CZhData::gan(qm_->get_dp()[g]);
    std::wstring szprint = di;
    if (isJi) {
        szprint += szJi + L" ";
    } else {
        szprint += L"   ";
    }
    std::wcout << COLOR_YELLOW << szprint << COLOR_RESET;
}

void CCmdPrint::print_base()
{
    const CDateTime& solar = qm_->get_cal()->get_solar();
    const CDateTime& lunar = qm_->get_cal()->get_lunnar();
    std::wcout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::wcout << L"公元：" << COLOR_CYAN << std::setw(4) << std::setfill(L'0') << solar.date_.year_ << COLOR_RESET << L"年";
    std::wcout << COLOR_CYAN << std::setw(2) << std::setfill(L'0') << solar.date_.mon_ << COLOR_RESET << L"月";
    std::wcout << COLOR_CYAN << std::setw(2) << std::setfill(L'0') << solar.date_.day_ << COLOR_RESET << L"日 ";
    std::wcout << COLOR_CYAN << std::setw(2) << std::setfill(L'0') << solar.time_.h_ << COLOR_RESET << L"时";
    std::wcout << COLOR_CYAN << std::setw(2) << std::setfill(L'0') << solar.time_.m_ << COLOR_RESET << L"分";
    std::wcout << COLOR_CYAN << std::setw(2) << std::setfill(L'0') << solar.time_.s_ << COLOR_RESET << L"秒       ";
    std::wcout << COLOR_CYAN << CSP_VERSION << COLOR_RESET << std::endl;

    std::wcout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::wstring yearStr{};
    int lyear{};
    if (lunar.date_.year_ < 0) {
        lyear = -lunar.date_.year_;
        yearStr.append(L"前");
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
    std::wcout << L"阴历：" << COLOR_CYAN << yearStr << COLOR_RESET << L"年 ";
    std::wcout << COLOR_CYAN << CZhData::mon(lunar.date_.mon_ - 1) << COLOR_RESET << L"月";
    std::wcout << COLOR_CYAN << CZhData::lunar_day(lunar.date_.day_ - 1) << COLOR_RESET << L"日 ";
    std::wcout << COLOR_CYAN << CZhData::zhi((lunar.time_.h_ + 1) / 2 % 12) << COLOR_RESET << L"时\n";

    // std::wcout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
    std::wcout << L"干支：" << COLOR_YELLOW << CZhData::gan(gz.yg_) << L"  ";
    std::wcout << CZhData::gan(gz.mg_) << L"  ";
    std::wcout << CZhData::gan(gz.dg_) << L"  ";
    std::wcout << CZhData::gan(gz.hg_) << L"  \n";
    std::wcout << L"      " << CZhData::zhi(gz.yz_) << L"  ";
    std::wcout << CZhData::zhi(gz.mz_) << L"  ";
    std::wcout << CZhData::zhi(gz.dz_) << L"  ";
    std::wcout << CZhData::zhi(gz.hz_) << L"      " << COLOR_RESET;

    switch (type_) {
        case 1: {
            std::wcout << COLOR_CYAN << L"<时家转盘超接置润法>" << COLOR_RESET << std::endl;
            break;
        }
        case 2: {
            std::wstring szYueJiang = CZhData::zhi(12 - qm_->get_cal()->get_lunnar().date_.mon_);
            std::wcout << COLOR_CYAN << L" (月将:" << szYueJiang << L")(时家阴盘)" << COLOR_RESET << std::endl;
            break;
        }
        case 3: {
            std::wcout << COLOR_CYAN << L"    <时家转盘拆补法>" << COLOR_RESET << std::endl;
            break;
        }
        case 4: {
            std::wcout << COLOR_CYAN << L"    <时家转盘茅山法>" << COLOR_RESET << std::endl;
            break;
        }
        default:
            break;
    }

    std::wcout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::wcout << L"值符：" << COLOR_YELLOW << CZhData::jx(qm_->get_jx_pre()[qm_->get_duty()]) << COLOR_RESET;
    std::wcout << L"  值使：" << COLOR_YELLOW << CZhData::bm(qm_->get_bm_pre()[qm_->get_duty()]) << L" ";

    std::wstring szYuan;
    switch (qm_->get_yuan()) {
        case 0:
            szYuan = L"手动定局";
            break;
        case 1:
            szYuan = L"下元";
            break;
        case 2:
            szYuan = L"中元";
            break;
        case 3:
            szYuan = L"上元";
            break;
        default:
            break;
    }
    std::wstring jieQi = CZhData::jq(qm_->get_jq());
    if (qm_->get_yuan() == 0) {
        szYuan = L"   [" + szYuan + L"]";
    } else {
        szYuan = L"   [" + jieQi + szYuan + L"]";
    }
    std::wcout << COLOR_YELLOW << szYuan;

    std::wstring dun;
    if (qm_->is_yin()) {
        dun = L"[阴遁" + CZhData::num(qm_->get_js()) + L"局]\n";
    } else {
        dun = L"[阳遁" + CZhData::num(qm_->get_js()) + L"局]\n";
    }
    std::wcout << COLOR_YELLOW << dun << COLOR_RESET;
    std::wcout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
}

void CCmdPrint::print_other()
{
    std::wcout << L"旬空：";
    std::wstring szXunkong;
    szXunkong += CZhData::zhi(qm_->get_xk()[0]) + CZhData::zhi(qm_->get_xk()[1]) + L"   ";
    szXunkong += CZhData::zhi(qm_->get_xk()[2]) + CZhData::zhi(qm_->get_xk()[3]) + L"   ";
    szXunkong += CZhData::zhi(qm_->get_xk()[4]) + CZhData::zhi(qm_->get_xk()[5]) + L"   ";
    szXunkong += CZhData::zhi(qm_->get_xk()[6]) + CZhData::zhi(qm_->get_xk()[7]) + L"   \n";
    std::wcout << COLOR_YELLOW << szXunkong << COLOR_RESET;
    std::wcout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::wstring szJieName;
    const auto& first_jq = qm_->get_cal()->get_jie().jq[2];
    const auto& second_jq = qm_->get_cal()->get_jie().jq[3];
    szJieName += L"[" + CZhData::jq(first_jq.index_) + L"]：";
    std::wcout << szJieName;
    wchar_t szTem[128]{};
    std::swprintf(szTem, sizeof(szTem), L"%02d-%02d %02d:%02d:%02d", first_jq.dt_.date_.mon_, first_jq.dt_.date_.day_,
                  first_jq.dt_.time_.h_, first_jq.dt_.time_.m_, first_jq.dt_.time_.s_);
    std::wcout << COLOR_YELLOW << std::wstring(szTem) << COLOR_RESET;
    szJieName.clear();
    szJieName += L"  [" + CZhData::jq(second_jq.index_) + L"]：";
    std::wcout << szJieName;
    std::swprintf(szTem, sizeof(szTem), L"%02d-%02d %02d:%02d:%02d", second_jq.dt_.date_.mon_, second_jq.dt_.date_.day_,
                  second_jq.dt_.time_.h_, second_jq.dt_.time_.m_, second_jq.dt_.time_.s_);
    std::wcout << COLOR_YELLOW << std::wstring(szTem) << COLOR_RESET << std::endl;
    std::wcout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
    std::wcout << COLOR_YELLOW << L"https://github.com/taynpg/csp" << COLOR_RESET << std::endl;
    std::wcout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
}

void CCmdPrint::run(cppbox::CQimen* qm, int nType)
{
    type_ = nType;
    this->qm_ = qm;

    print_base();
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_one(4);
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_one(3);
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_one(2);
    std::wcout << COLOR_GREEN << L"|\n" << COLOR_RESET;

    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_two(4);
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_two(3);
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_two(2);
    std::wcout << COLOR_GREEN << L"|\n" << COLOR_RESET;

    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_three(4);
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_three(3);
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_three(2);
    std::wcout << COLOR_GREEN << L"|\n" << COLOR_RESET;
    std::wcout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_one(5);
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_one(8);
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_one(1);
    std::wcout << COLOR_GREEN << L"|\n" << COLOR_RESET;

    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_two(5);
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_two(8);
    std::wcout << COLOR_GREEN << "|" << COLOR_RESET;
    print_two(1);
    std::wcout << COLOR_GREEN << L"|\n" << COLOR_RESET;

    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_three(5);
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_three(8);
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_three(1);
    std::wcout << COLOR_GREEN << L"|\n" << COLOR_RESET;
    std::wcout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_one(6);
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_one(7);
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_one(0);
    std::wcout << COLOR_GREEN << L"|\n" << COLOR_RESET;

    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_two(6);
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_two(7);
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_two(0);
    std::wcout << COLOR_GREEN << L"|\n" << COLOR_RESET;

    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_three(6);
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_three(7);
    std::wcout << COLOR_GREEN << L"|" << COLOR_RESET;
    print_three(0);
    std::wcout << COLOR_GREEN << L"|\n" << COLOR_RESET;
    std::wcout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
    print_other();
}
