#ifndef BOX_QIMEN_BASE_
#define BOX_QIMEN_BASE_

#include "calendar_base.h"

namespace cppbox {

using cb = CCalender;
constexpr int g_num = 9;
enum class QIMEN_STYLE {
    SHIJIA_ZHUANPAN_CHAOJIE_ZHIRUN = 1,   // 时家转盘，超接置润法
    SHIJIA_ZHUANPAN_YINPAN,               // 时家转盘，阴盘
    SHIJIA_ZHUANPAN_CHAIBU,               // 时家转盘，拆补
    SHIJIA_ZHUANPAN_MAOSHAN,              // 时家转盘，茅山
    NOT_DEFINE                            // 未定义
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

// 排盘的参数信息
struct QiParam {
    int ju_{};               // 局数，默认: 0 为程序自动选择，其他指定。
    CDateTime datetime_{};   // 时间日期
};

class CQimen
{
public:
    CQimen();
    virtual ~CQimen();

public:
    // 初始化
    virtual void base_init();
    // 进行计算
    virtual bool base_run(const QiParam& info, CalendarType type);
    virtual bool run(const QiParam& info, CalendarType type) = 0;
    //
    virtual const char* getLastError() const;
    // 设置各位置的五行(金水木火土 数字代表 12345)
    virtual void set_wx();
    // 设置九宫的原始九星位置[位置-九星]
    virtual void set_jx_pre();
    // 设置九星的旋转位置[相对顺序]
    virtual void set_jx_turn();
    // 设置九宫的数字对应[位置-数字(数字从0开始)]
    virtual void set_gong_base();
    // 设置八门的初始位置[位置-八门]
    virtual void set_bm_pre();
    // 设置八门的旋转位置
    virtual void set_bm_turn();
    // 设置八神的初始位置[位置-八神]
    virtual void set_bs_pre();
    // 设置寄宫[固定位置](这里的寄宫是各家自家的卦数)
    virtual void set_jg(int nGong);
    // 设置十二地支位置对应
    virtual void setDizhi();
    // 获取六十甲子下标
    static int get_jz(int nTianIndex, int nDiIndex);
    // 查找内容所在的下标
    static int get_index(const int* pData, int nSize, int nValue);
    // 获取下 n 个值所在的原始位置(正值代表向下数，负值代表向上数)
    // static int getNextPosition(int* pData, int nSize, int nCur, int nValue);
protected:
    // 设置初始内容之后，执行此函数，自动生成所需的信息。
    void prepare();

public:
    // 排地盘
    virtual void gen_dipan() = 0;
    // 查找值符，值使
    virtual void gen_zhi() = 0;
    // 排九星
    virtual void gen_jx() = 0;
    // 排八门
    virtual void gen_bm() = 0;
    // 排八神
    virtual void gen_bs() = 0;
    // 排天盘
    virtual void gen_tp() = 0;
    // 排旬空马星
    virtual void gen_other() = 0;

public:
    // 获取天盘 (size = 9)
    const int* get_tp() const;
    // 获取地盘 (size = 9)
    const int* get_dp() const;
    // 获取九星 (size = 9)
    const int* get_jx() const;
    const int* get_jx_pre() const;
    // 获取八门 (size = 9)
    const int* get_bm() const;
    const int* get_bm_pre() const;
    // 获取八神 (size = 9)
    const int* get_bs() const;
    // 获取马星
    int get_mx() const;
    // 获取空亡 (size = 2)
    const int* get_kw() const;
    // 获取旬空信息
    const int* get_xk() const;
    // 获取日历
    const std::shared_ptr<CCalender> get_cal() const;
    // 获取值班人员
    int get_duty() const;
    // 获取寄宫
    int get_jg() const;
    // 获取三元
    int get_yuan() const;
    // 获取当日节气
    int get_jq() const;
    // 获取局数
    int get_js() const;
    // 获取是否是阴遁
    bool is_yin() const;
    // 获取五行(金水木火土 数字代表 12345)
    const int* get_wx() const;
    // 获取卦序号
    const int* get_guaxu() const;
    // 获取地支与位置的对应关系
    const int* get_zhi_pos() const;
    // 是否是五不遇时
    bool is_wby() const;

protected:
    int jushu_{};            // 局数
    int jx_re_[g_num]{};     // 九星的计算结果
    int jx_pre_[g_num]{};    // 九星的原始位置
    int jx_turn_[g_num]{};   // 九星的旋转位置，不足9个的从后向前留空
    int pos2gua_[g_num]{};   // 位置转卦数
    int bm_pre_[g_num]{};    // 八门的原始位置
    int bm_re_[g_num]{};     // 八门的计算结果
    int bm_turn_[g_num]{};   // 八门的旋转位置
    int bs_pre_[g_num]{};    // 八神的原始位置
    int bs_re_[g_num]{};     // 八神的计算结果
    int t_[g_num]{};         // 天盘
    int d_[g_num]{};         // 地盘 [定义位置：10天干]
    int wx_[g_num]{};        // 五行
    int jg_{};               // 天禽星寄宫
    int mx_{};               // 马星位置
    int kw_[2]{};            // 空亡位置
    int xk_[8]{};            // 寻空位置
    bool is_yin_{};          // 是否是阴遁
    int yuan_{};             // 三元 0 手动, 1下，2中，3上
    bool auto_{};            // 是否是自动排局
    bool wbys_{false};       // 是否是五不遇时
    int zf_{};               // 值符
    int zs_{};               // 值使
    int jq_{};               // 当日节气
    int jz_{};               // 当日六十甲子
protected:
    CDateTime datetime_;
    std::shared_ptr<CCalender> pcal_{};   // 日历实例
    int gua2pos_[g_num]{};                // 卦数转位置
    int sanhe_[12]{};                     // 地支三和
    int dzc_[12]{};                       // 地支相冲
    int zhi_[12]{};                       // 十二地支的位置
    CalendarType cal_type_;               // 日历的类型
    char err_[512]{};
};

// 奇门工厂类
class CQimenFactory
{
private:
    CQimenFactory() = default;
    ~CQimenFactory() = default;

public:
    // 获取实例
    static std::shared_ptr<CQimen> createInstance(QIMEN_STYLE type);
};
}   // namespace cppbox
#endif
