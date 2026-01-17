#ifndef QM_SHIJIA_CHAOJIEZHIRUN
#define QM_SHIJIA_CHAOJIEZHIRUN

#include "qimen.h"
#include <unordered_map>
#include <vector>

namespace csp {

// V1 : 转盘 时家 超接置润法

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

struct OneDay {
    OneDay() = default;
    OneDay(const DateTime& adt, int ajq, int agz);
    DateTime dt{};
    int jq{};
    int ganzhi{};
};

class QimenV1 : public Qimen
{
public:
    QimenV1();
    virtual ~QimenV1() = default;

public:
    // 排盘
    bool generate(const DateTime& dt, int ju) override;

    /// @brief 排地盘
    void cal_dipan() override;

    /// @brief 计算当值人员
    void cal_duty() override;

    /// @brief 排九星
    void cal_jiuxing() override;

    /// @brief 排八门
    void cal_bamen() override;

    /// @brief 排八神
    void cal_bashen() override;

    /// @brief 排天盘
    void cal_tianpan() override;

    /// @brief 排其他信息（如空亡、马星）
    void cal_other() override;

private:
    // 推演一年的日历
    bool inference();

    // 在一年的日历中查找当天的信息
    bool search_day(const DateTime& datetime, OneDay& o);

    // 获取当月节气信息
    bool cur_month_jq(const DateTime& dt, DateTime& jie, int& jiazi, int& dif);

    // 保存某段范围的日期信息
    void save_part(DateTime& sdt, int& upper, int& jiazi, int pur, int days, bool addJie = true);

    // 获取给定甲子的两个寻空
    std::pair<int, int> cal_xunkong(int jiazi);

    // 打印
    void print();

private:
    bool cal_ju(const DateTime& dt);

protected:
    int juqi_[24]{};

    std::vector<OneDay> year_datas_{};
    std::unordered_map<int, int> wby_{};
};
}   // namespace csp

#endif