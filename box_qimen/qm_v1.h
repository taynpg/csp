#ifndef QIMEN_HEADER_
#define QIMEN_HEADER_

#include "qm_base.h"
#include <vector>

namespace cppbox {

    /*
     *  转盘置润法的排盘方法：
     *      一旬十个时辰、由一个值符看管、九星中的一个星。
     *      1.地盘的排法（超接法定遁）
     *      2.值符就是：当前时干旬头甲，在地盘哪个宫，这个宫的原始星就是值符。
     *      3.值使同值符相同的找法。
     *      4.转动星盘：值符随时干，即值符放在时干的位置。
     *      5.转动门盘：值使随时支，即值使放在从旬头按阳顺阴逆的方法排列。
     *
     * */

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
    bool Run(const QiParam& info, CalendarType type) override;
protected:
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
    // void printYearDay();
    // 获取两个日期的天数差
    static int getDayDifference(const CDate& dateA, const CDate& dateB);
    // 获取给定甲子的两个寻空
    static void getXunKong(int nJiazi, int& nKongA, int& nKongB);
private:
    std::vector<OneDay *>    m_pOneYear;     // 四柱节气的一年推算
    bool                     m_zhirun{};
    int                      m_nJuQi[24]{};    // 节气和局的关系对照表
};

}
#endif