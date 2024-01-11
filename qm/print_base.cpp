#include "print_base.h"

#include <iomanip>
#include <iostream>

#include "../csp_base.hpp"
#include "zh_lang.h"

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

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

void CCmdPrint::PrintOne(int nGong)
{
    if (nGong == 8) {
        std::cout << "              ";
        return;
    }
    std::string bashen = CZhData::ZhBaShen(m_qm->getBaShen()[nGong]);
    std::cout << COLOR_WHITE << "  " << bashen << " ";
    if (nGong == m_qm->getMaXing()) {
        if (nGong == m_qm->getKong()[0] || nGong == m_qm->getKong()[1]) {
            std::cout << COLOR_CYAN << " ("
                      << CZhData::ZhMaXing() + CZhData::ZhKongWang() + ")"
                      << COLOR_RESET;
        } else {
            std::cout << COLOR_CYAN << " (" << CZhData::ZhMaXing() + ")  "
                      << COLOR_RESET;
        }
    } else if (nGong == m_qm->getKong()[0] || nGong == m_qm->getKong()[1]) {
        std::cout << COLOR_CYAN << " (" << CZhData::ZhKongWang() + ")  "
                  << COLOR_RESET;
    } else {
        std::cout << "       ";
    }
}

void CCmdPrint::PrintTwo(int nGong)
{
    if (nGong == 8) {
        std::cout << "              ";
        return;
    }
    std::string jiuxing = CZhData::ZhJiuXing(m_qm->getJiuXing()[nGong]);
    std::cout << COLOR_GREEN << "  " << jiuxing << "   ";
    bool        isJi = false;
    std::string szJi;
    // 如果此宫的星是寄宫星，那么就寄到这里
    if (m_qm->getJiuXing()[nGong] == m_qm->getJiuXingPre()[m_qm->getJiGong()]) {
        isJi = true;
        szJi = CZhData::ZhGan(m_qm->getDiPan()[8]);
    }
    std::string tian = CZhData::ZhGan(m_qm->getTianPan()[nGong]);
    std::string szprint = tian;
    if (isJi) {
        szprint += szJi + " ";
    } else {
        szprint += "   ";
    }
    std::cout << COLOR_YELLOW << szprint << COLOR_RESET;
}

void CCmdPrint::PrintThree(int nGong)
{
    if (nGong == 8) {
        std::cout << "              ";
        return;
    }
    std::string bamen = CZhData::ZhBaMen(m_qm->getBaMen()[nGong]);
    std::cout << COLOR_CYAN << "  " << bamen << "   ";
    bool        isJi = false;
    std::string szJi;
    // 如果此宫的星是寄宫星，那么就寄到这里
    if (nGong == m_qm->getJiGong()) {
        isJi = true;
        szJi = CZhData::ZhGan(m_qm->getDiPan()[8]);
    }
    std::string di = CZhData::ZhGan(m_qm->getDiPan()[nGong]);
    std::string szprint = di;
    if (isJi) {
        szprint += szJi + " ";
    } else {
        szprint += "   ";
    }
    std::cout << COLOR_YELLOW << szprint << COLOR_RESET;
}

void CCmdPrint::PrintBase()
{
    const CDateTime& solar = m_qm->getCalendar()->getDateTime();
    const CDateTime& lunar = m_qm->getCalendar()->getLunarDateTime();
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::cout << "公元：" << COLOR_CYAN << std::setw(4) << std::setfill('0')
              << solar.m_date.m_nYear << COLOR_RESET << "年";
    std::cout << COLOR_CYAN << std::setw(2) << std::setfill('0')
              << solar.m_date.m_nMon << COLOR_RESET << "月";
    std::cout << COLOR_CYAN << std::setw(2) << std::setfill('0')
              << solar.m_date.m_nDay << COLOR_RESET << "日 ";
    std::cout << COLOR_CYAN << std::setw(2) << std::setfill('0')
              << solar.m_time.m_nHour << COLOR_RESET << "时";
    std::cout << COLOR_CYAN << std::setw(2) << std::setfill('0')
              << solar.m_time.m_nMin << COLOR_RESET << "分";
    std::cout << COLOR_CYAN << std::setw(2) << std::setfill('0')
              << solar.m_time.m_nSec << COLOR_RESET << "秒       ";
    std::cout << COLOR_CYAN << CSP_VERSION << COLOR_RESET << std::endl;

    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::string yearStr{};
    int         lyear{};
    if (lunar.m_date.m_nYear < 0) {
        lyear = -lunar.m_date.m_nYear;
        yearStr.append("前");
    } else {
        lyear = lunar.m_date.m_nYear;
    }

    bool add = false;
    int  oya = lyear / 1000;
    int  oyb = lyear / 100 % 10;
    int  oyc = lyear / 10 % 10;
    int  oyd = lyear % 10;

    if (oya != 0) add = true;
    if (add) yearStr.append(CZhData::ZhNumber(oya));
    if (oyb != 0) add = true;
    if (add) yearStr.append(CZhData::ZhNumber(oyb));
    if (oyc != 0) add = true;
    if (add) yearStr.append(CZhData::ZhNumber(oyc));
    if (oyd != 0) add = true;
    if (add) yearStr.append(CZhData::ZhNumber(oyd));

    const CGanZhi& gz = m_qm->getCalendar()->getSizhu();
    std::cout << "阴历：" << COLOR_CYAN << yearStr << COLOR_RESET << "年 ";
    std::cout << COLOR_CYAN << CZhData::ZhMon(lunar.m_date.m_nMon - 1)
              << COLOR_RESET << "月";
    std::cout << COLOR_CYAN << CZhData::ZhLunarDay(lunar.m_date.m_nDay - 1)
              << COLOR_RESET << "日 ";
    std::cout << COLOR_CYAN
              << CZhData::ZhZhi((lunar.m_time.m_nHour + 1) / 2 % 12)
              << COLOR_RESET << "时\n";

    // std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
    std::cout << "干支：" << COLOR_YELLOW << CZhData::ZhGan(gz.m_nYGan) << "  ";
    std::cout << CZhData::ZhGan(gz.m_nMGan) << "  ";
    std::cout << CZhData::ZhGan(gz.m_nDGan) << "  ";
    std::cout << CZhData::ZhGan(gz.m_nHGan) << "  \n";
    std::cout << "      " << CZhData::ZhZhi(gz.m_nYZhi) << "  ";
    std::cout << CZhData::ZhZhi(gz.m_nMZhi) << "  ";
    std::cout << CZhData::ZhZhi(gz.m_nDZhi) << "  ";
    std::cout << CZhData::ZhZhi(gz.m_nHZhi) << "      " << COLOR_RESET;

    switch (m_type) {
        case 1: {
            std::cout << COLOR_CYAN << "<时家转盘超接置润法>" << COLOR_RESET
                      << std::endl;
            break;
        }
        case 2: {
            std::string szYueJiang = CZhData::ZhZhi(
                12 - m_qm->getCalendar()->getLunarDateTime().m_date.m_nMon);
            std::cout << COLOR_CYAN << " (月将:" << szYueJiang << ")(时家阴盘)"
                      << COLOR_RESET << std::endl;
            break;
        }
        case 3: {
            std::cout << COLOR_CYAN << "    <时家转盘拆补法>" << COLOR_RESET
                      << std::endl;
            break;
        }
        default:
            break;
    }

    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::cout << "值符：" << COLOR_YELLOW
              << CZhData::ZhJiuXing(m_qm->getJiuXingPre()[m_qm->getDuty()])
              << COLOR_RESET;
    std::cout << "  值使：" << COLOR_YELLOW
              << CZhData::ZhBaMen(m_qm->getBaMenPre()[m_qm->getDuty()]) << " ";

    std::string szYuan;
    switch (m_qm->getYuan()) {
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
    std::string jieQi = CZhData::ZhJieQi(m_qm->getJieQi());
    if (m_qm->getYuan() == 0) {
        szYuan = "   [" + szYuan + "]";
    } else {
        szYuan = "   [" + jieQi + szYuan + "]";
    }
    std::cout << COLOR_YELLOW << szYuan;

    std::string dun;
    if (m_qm->getIsYinDun()) {
        dun = "[阴遁" + CZhData::ZhNumber(m_qm->getJushu()) + "局]\n";
    } else {
        dun = "[阳遁" + CZhData::ZhNumber(m_qm->getJushu()) + "局]\n";
    }
    std::cout << COLOR_YELLOW << dun << COLOR_RESET;
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
}

void CCmdPrint::PrintOther()
{
    std::cout << "旬空：";
    std::string szXunkong;
    szXunkong += CZhData::ZhZhi(m_qm->getXunKong()[0]) +
                 CZhData::ZhZhi(m_qm->getXunKong()[1]) + "   ";
    szXunkong += CZhData::ZhZhi(m_qm->getXunKong()[2]) +
                 CZhData::ZhZhi(m_qm->getXunKong()[3]) + "   ";
    szXunkong += CZhData::ZhZhi(m_qm->getXunKong()[4]) +
                 CZhData::ZhZhi(m_qm->getXunKong()[5]) + "   ";
    szXunkong += CZhData::ZhZhi(m_qm->getXunKong()[6]) +
                 CZhData::ZhZhi(m_qm->getXunKong()[7]) + "   \n";
    std::cout << COLOR_YELLOW << szXunkong << COLOR_RESET;
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::string szJieName;
    szJieName += "[" +
                 CZhData::ZhJieQi(m_qm->getCalendar()->getJieFirst().index) +
                 "]：";
    std::cout << szJieName;
    char szTem[128]{};
    std::snprintf(szTem, sizeof(szTem), "%02d-%02d %02d:%02d:%02d",
                  m_qm->getCalendar()->getJieFirst().datetime.m_date.m_nMon,
                  m_qm->getCalendar()->getJieFirst().datetime.m_date.m_nDay,
                  m_qm->getCalendar()->getJieFirst().datetime.m_time.m_nHour,
                  m_qm->getCalendar()->getJieFirst().datetime.m_time.m_nMin,
                  m_qm->getCalendar()->getJieFirst().datetime.m_time.m_nSec);
    std::cout << COLOR_YELLOW << std::string(szTem) << COLOR_RESET;
    szJieName.clear();
    szJieName += "  [" +
                 CZhData::ZhJieQi(m_qm->getCalendar()->getJieSecond().index) +
                 "]：";
    std::cout << szJieName;
    std::snprintf(szTem, sizeof(szTem), "%02d-%02d %02d:%02d:%02d",
                  m_qm->getCalendar()->getJieSecond().datetime.m_date.m_nMon,
                  m_qm->getCalendar()->getJieSecond().datetime.m_date.m_nDay,
                  m_qm->getCalendar()->getJieSecond().datetime.m_time.m_nHour,
                  m_qm->getCalendar()->getJieSecond().datetime.m_time.m_nMin,
                  m_qm->getCalendar()->getJieSecond().datetime.m_time.m_nSec);
    std::cout << COLOR_YELLOW << std::string(szTem) << COLOR_RESET << std::endl;
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
}

void CCmdPrint::Run(cppbox::CQimen* qm, int nType)
{
    m_type = nType;
#ifdef _WIN32
    // 获取标准输出句柄
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // 启用 ANSI 转义代码支持
    DWORD mode;
    GetConsoleMode(hConsole, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, mode);
#endif
    this->m_qm = qm;

    PrintBase();
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintOne(4);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintOne(3);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintOne(2);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;

    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintTwo(4);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintTwo(3);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintTwo(2);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;

    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintThree(4);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintThree(3);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintThree(2);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintOne(5);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintOne(8);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintOne(1);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;

    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintTwo(5);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintTwo(8);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintTwo(1);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;

    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintThree(5);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintThree(8);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintThree(1);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;

    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintOne(6);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintOne(7);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintOne(0);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;

    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintTwo(6);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintTwo(7);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintTwo(0);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;

    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintThree(6);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintThree(7);
    std::cout << COLOR_GREEN << "|" << COLOR_RESET;
    PrintThree(0);
    std::cout << COLOR_GREEN << "|\n" << COLOR_RESET;
    std::cout << COLOR_GREEN << SPLIT_LINE << COLOR_RESET << std::endl;
    PrintOther();
}
