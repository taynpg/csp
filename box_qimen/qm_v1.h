#ifndef QIMEN_HEADER_
#define QIMEN_HEADER_

#include "qm_base.h"
#include <vector>

namespace cppbox {

// 一天的信息
struct OneDay {
    CDate   date;
    int     nJieIndex;
    int     nGanZhiIndex;
};

// V1 : 转盘 时家 超接置润法
class CQiMenV1 : public CQimen {
public:
    CQiMenV1();
    ~CQiMenV1() override;
public:
    // 传入日期数据
    bool Run(const QiInfomation& info, CalendarType type) override;
private:
    // 推算一年的日历
    void inferenceDate();
    // 排地盘
    void genDiPan() override;
    // 查找值符，值使
    void genZhi() override;
    // 排九星
    void genJiuXing() override;
    // 排八门
    void genBaMen() override;
    // 排八神
    void genBaShen() override;
    // 排天盘
    void genTianPan() override;
    // 排旬空马星
    void genOther() override;
private:
    void savePart(CDate& date, int& nUpper, int& nJiazi, int nPur, int nDays);
    void saveDay(const CDate& date, int nJie, int& nJiazi);
    void getCurrentMonJie(const CDateTime& datetime, CDateTime& datetimeJie, int& nJiazi, int& nFutouDiff);
    // 在一年的日历中查找当天的信息
    OneDay* searchDay(const CDateTime& datetime);
    // 打印所推演的一年日历
    void printYearDay();
    // 获取两个日期的天数差
    int getDayDifference(const CDate& dateA, const CDate& dateB);
    // 获取给定甲子的两个寻空
    void getXunKong(int nJiazi, int& nKongA, int& nKongB);
private:
    std::vector<OneDay *>    m_pOneYear;     // 四柱节气的一年推算
    bool                     m_zhirun{};
    int                      m_nJuQi[24]{};    // 节气和局的关系对照表
};

}

#endif