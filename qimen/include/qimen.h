#ifndef QIMEN_H
#define QIMEN_H

#include <calendar.h>

namespace csp {
constexpr int gn = 9;

enum class QimenType {
    QM_TYPE_SJ_ZHIRUN = 1,   // 时家转盘，超接置润法
    QM_TYPE_SJ_YIN,          // 时家转盘，阴盘
    QM_TYPE_SJ_CHAIBU,       // 时家转盘，拆补
    QM_TYPE_SJ_MAOSHAN       // 时家转盘，茅山
};

/*
 *   本类中涉及的下标默认内容：
 *   以 Pre 结尾的是原始位置：
 *   以 Turn 结尾的是旋转位置：
 *   以 Re 结尾的是计算结果
 *
 *   排盘逻辑，无论各派别的定义，排法如何，都基于本程序的原始定义，
 *   《排盘前设置好位置和各个符号的初始对应关系》
 *   最终生成的结果均已本程序定义的位置排序
 *
 *      -----------------------------------------------------
 *      |                 |                |                |
 *      |     位置 4      |     位置 3     |     位置 2     |
 *      |                 |                |                |
 *      |                 |                |                |
 *      -----------------------------------------------------
 *      |                 |                |                |
 *      |     位置 5      |    位置 8      |     位置 1     |
 *      |                 |                |                |
 *      |                 |                |                |
 *      -----------------------------------------------------
 *      |                 |                |                |
 *      |     位置 6      |     位置 7     |    起始位置 0  |
 *      |                 |                |                |
 *      |                 |                |                |
 *      -----------------------------------------------------
 *
 *
 *   下面是固定定义，不同的盘需要先设定它们的原始位置和旋转位置：
 *       -1     0     1     2     3      4     5     6     7      8
 *      为空  天蓬  天芮  天冲   天辅  天禽  天心  天柱   天任   天英
 *      为空  休门  死门  伤门   杜门  死门  开门  惊门   生门   景门
 *      为空  值符  腾蛇  太阴   六合  腾蛇  白虎  玄武   九地   九天
 *      为空  坎卦  坤卦  震卦   巽卦  中五  乾卦  兑卦   艮卦   离卦
 */

struct QimenData {
    CSPT ju{};            // 局数
    CSPT jiuxr[gn]{};     // 九星的计算结果
    CSPT jiuxp[gn]{};     // 九星的原始位置
    CSPT jiuxt[gn]{};     // 九星的旋转位置，不足9个的从后向前留空
    CSPT pos2gua[gn]{};   // 位置转卦数
    CSPT gua2pos[gn]{};   // 卦数转位置
    CSPT bamenp[gn]{};    // 八门的原始位置
    CSPT bamenr[gn]{};    // 八门的计算结果
    CSPT bament[gn]{};    // 八门的旋转位置
    CSPT bashenp[gn]{};   // 八神的原始位置
    CSPT bashenr[gn]{};   // 八神的计算结果
    CSPT tp[gn]{};        // 天盘
    CSPT dp[gn]{};        // 地盘 [定义位置：10天干]
    CSPT wuxing[gn]{};    // 五行
    CSPT jigong{};        // 天禽星寄宫
    CSPT maxing{};        // 马星位置
    CSPT kongw[2]{};      // 空亡位置
    CSPT xunkong[8]{};    // 寻空位置
    bool is_yin{};        // 是否是阴遁
    CSPT yuan{};          // 三元 -1没有，0 手动, 1下，2中，3上
    bool is_auto{true};   // 是否是自动排局
    bool wubuyu{false};   // 是否是五不遇时
    CSPT duty{};          // 当值
    CSPT jieq{};          // 当日节气
    CSPT jiazi{};         // 当日六十甲子
    CSPT zhi[12]{};
    CSPT sanhe_[12]{};    // 地支三和
    CSPT dzcong_[12]{};   // 地支相冲
    CSPT yuejiang_{};     // 月将（如果有的话）
};

class Qimen
{
public:
    Qimen();
    virtual ~Qimen() = default;

public:
    /// @brief 获取六十甲子的下标
    /// @param t 天盘下标
    /// @param d 地盘下标
    /// @return
    static CSPT get_jiazi_index(CSPT t, CSPT d);

    /// @brief 获取某个值的位置
    /// @param d 数组
    /// @param size 数组大小
    /// @param value 要寻找的值
    /// @return
    static CSPT get_index(const CSPT* d, CSPT size, CSPT value);

    /// @brief 获取 奇门 实例
    /// @param type
    /// @return
    static std::shared_ptr<Qimen> instance(QimenType type);

public:
    /// @brief 当某个函数执行失败时，获取错误信息。
    /// @return
    const char* get_error() const;

    /// @brief 进行排盘
    /// @param dt 时间日期
    /// @param ju 指定局数。0 为自动，其他需要在 [-9, 9] 的范围。
    /// @param ct 使用日历类型
    /// @return
    virtual bool generate(const DateTime& dt, int ju, CalendarType ct);

    /// @brief 设置信息，并检查。
    /// @param dt 日期
    /// @param ct 日历类型
    /// @return
    virtual bool set_and_check(const DateTime& dt, int ju, CalendarType ct);

    /// @brief 设置各个位置上的五行 (金水木火土 数字代表 12345)
    virtual void set_wuxing();

    /// @brief 设置九宫的原始九星位置
    virtual void set_jiuxing_pre();

    /// @brief 设置九星的旋转位置[相对顺序]
    virtual void set_jiuxing_turn();

    /// @brief 设置九宫的数字对应[位置-数字(数字从0开始)]
    virtual void set_gong_pos();

    /// @brief 设置八门的初始位置[位置-八门]
    virtual void set_bamen_pre();

    /// @brief 设置八门的旋转位置
    virtual void set_bamen_turn();

    /// @brief 设置八神的初始位置[位置-八神]
    virtual void set_bashen_pre();

    /// @brief 设置寄宫[固定位置](这里的寄宫是各家自家的卦数)
    /// @param gong 卦数
    virtual void set_jigong(CSPT gong);

    /// @brief 设置十二地支位置对应
    virtual void set_dizhi();

    virtual std::shared_ptr<Calendar> get_cal() const;

public:
    /// @brief 排地盘
    virtual void cal_dipan() = 0;

    /// @brief 计算当值人员
    virtual void cal_duty() = 0;

    /// @brief 排九星
    virtual void cal_jiuxing() = 0;

    /// @brief 排八门
    virtual void cal_bamen() = 0;

    /// @brief 排八神
    virtual void cal_bashen() = 0;

    /// @brief 排天盘
    virtual void cal_tianpan() = 0;

    /// @brief 排其他信息（如空亡、马星）
    virtual void cal_other() = 0;

public:
    QimenData get_result() const;

protected:
    QimenData data_{};

    DateTime dt_{};
    std::shared_ptr<Calendar> cal_{};   // 日历实例
    CalendarType ctype_{};              // 日历的类型
    char err_[1024]{};
};
}   // namespace csp
#endif