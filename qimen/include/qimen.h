#ifndef QIMEN_H
#define QIMEN_H

#include <cstdint>
#include <memory>
#include <tyme.h>

namespace csp {
constexpr int gn = 9;

enum class QimenType {
    QM_TYPE_SJ_ZHIRUN = 1,   // 时家转盘，超接置润法
    QM_TYPE_SJ_YIN,          // 时家转盘，阴盘
    QM_TYPE_SJ_CHAIBU,       // 时家转盘，拆补
    QM_TYPE_SJ_MAOSHAN       // 时家转盘，茅山
};

struct DateTime {
    DateTime() = default;
    DateTime(int y, int m, int d, int h, int mi, int s);
    int year{};   // 年
    int mon{};    // 月
    int day{};    // 日
    int hour{};   // 时
    int min{};    // 分
    int sec{};    // 秒
    int msec{};   // 毫秒
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
    int ju{};             // 局数
    int jiuxr[gn]{};      // 九星的计算结果
    int jiuxp[gn]{};      // 九星的原始位置
    int jiuxt[gn]{};      // 九星的旋转位置，不足9个的从后向前留空
    int pos2gua[gn]{};    // 位置转卦数
    int gua2pos[gn]{};    // 卦数转位置
    int bamenp[gn]{};     // 八门的原始位置
    int bamenr[gn]{};     // 八门的计算结果
    int bament[gn]{};     // 八门的旋转位置
    int bashenp[gn]{};    // 八神的原始位置
    int bashenr[gn]{};    // 八神的计算结果
    int tp[gn]{};         // 天盘
    int dp[gn]{};         // 地盘 [定义位置：10天干]
    int wuxing[gn]{};     // 五行
    int jigong{};         // 天禽星寄宫
    int maxing{};         // 马星位置
    int kongw[2]{};       // 空亡位置
    int xunkong[8]{};     // 寻空位置
    bool is_yin{};        // 是否是阴遁
    int yuan{};           // 三元 -1没有，0 手动, 1下，2中，3上
    bool is_auto{true};   // 是否是自动排局
    bool wubuyu{false};   // 是否是五不遇时
    int duty{};           // 当值
    int jieq{};           // 当日节气
    int jiazi{};          // 当日六十甲子
    int zhi[12]{};
    int sanhe_[12]{};    // 地支三和
    int dzcong_[12]{};   // 地支相冲
    int yuejiang_{};     // 月将（如果有的话）
    std::shared_ptr<tyme::SolarTime> dt_{};
};

struct JiaZi {
    int yi;
    int mi;
    int di;
    int hi;
};

class Qimen
{
public:
    Qimen();
    virtual ~Qimen() = default;

public:
    // ================== 这几个静态函数都是简单处理一些日历接口 ===========================
    static int mod(int base, int value);
    static DateTime now_time();
    static DateTime solar(const tyme::SolarTime& dt);
    static DateTime lunar(const tyme::SolarTime& dt);
    static tyme::SolarTime get_jq(const tyme::SolarTime& dt, int& index, bool is_sec);
    static JiaZi jiazi(const tyme::SolarTime& dt);
    static JiaZi jiaziNoHour(const tyme::SolarTime& dt);
    static tyme::SolarTime from(const DateTime& dt);
    static int between_days(const DateTime& dt1, const DateTime& dt2);
    static int between_secs(const DateTime& dt1, const DateTime& dt2);
    static DateTime next(const DateTime& dt, int seconds);
    // ====================================================================================

    /// @brief 获取六十甲子的下标
    /// @param t 天盘下标
    /// @param d 地盘下标
    /// @return
    static int get_jiazi_index(int t, int d);

    /// @brief 获取某个值的位置
    /// @param d 数组
    /// @param size 数组大小
    /// @param value 要寻找的值
    /// @return
    static int get_index(const int* d, int size, int value);

    /// @brief 获取 奇门 实例
    /// @param type
    /// @return
    static std::shared_ptr<Qimen> instance(QimenType type);

public:
    /// @brief 进行排盘
    /// @param dt 时间日期
    /// @param ju 指定局数。0 为自动，其他需要在 [-9, 9] 的范围。
    /// @param ct 使用日历类型
    /// @return
    virtual bool generate(const DateTime& dt, int ju);

    /// @brief 设置信息，并检查。
    /// @param dt 日期
    /// @param ct 日历类型
    /// @return
    virtual bool set_and_check(const DateTime& dt, int ju);

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
    virtual void set_jigong(int gong);

    /// @brief 设置十二地支位置对应
    virtual void set_dizhi();

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
};

}   // namespace csp

#endif