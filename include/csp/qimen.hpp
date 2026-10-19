// csp - 奇门遁甲排盘库
// 排盘基类。
#ifndef CSP_QIMEN_HPP
#define CSP_QIMEN_HPP

#include <memory>
#include <optional>
#include <string>

#include "csp/chart.hpp"
#include "csp/types.hpp"

namespace csp {

/*
 *   本程序中涉及的下标默认内容：
 *   以 Origin 结尾的是原始位置；
 *   以 Cycle 结尾的是旋转顺序；
 *   其余（stars / gates / deities）是计算结果。
 *
 *   排盘逻辑，无论各派别的定义，排法如何，都基于本程序的原始定义，
 *   《排盘前设置好位置和各个符号的初始对应关系》，
 *   最终生成的结果均以本程序定义的位置排序。
 *
 *      -----------------------------------------------------
 *      |                 |                |                |
 *      |     位置 4      |     位置 3     |     位置 2     |
 *      |                 |                |                |
 *      -----------------------------------------------------
 *      |                 |                |                |
 *      |     位置 5      |    位置 8      |     位置 1     |
 *      |                 |                |                |
 *      -----------------------------------------------------
 *      |                 |                |                |
 *      |     位置 6      |     位置 7     |    起始位置 0  |
 *      |                 |                |                |
 *      -----------------------------------------------------
 *
 *   固定定义如下，不同的盘需要先设定它们的原始位置和旋转位置：
 *       -1     0     1     2     3      4     5     6     7      8
 *      为空  天蓬  天芮  天冲   天辅  天禽  天心  天柱   天任   天英
 *      为空  休门  死门  伤门   杜门  死门  开门  惊门   生门   景门
 *      为空  值符  腾蛇  太阴   六合  腾蛇  白虎  玄武   九地   九天
 *      为空  坎卦  坤卦  震卦   巽卦  中五  乾卦  兑卦   艮卦   离卦
 */

/// 排盘基类。
///
/// 派生类只需实现「定局」和 8 个排盘步骤，排盘流程由 generate() 统一驱动。
class Qimen
{
public:
    Qimen();
    virtual ~Qimen() = default;

    Qimen(const Qimen&) = delete;
    Qimen& operator=(const Qimen&) = delete;

    /// 按盘式创建实例，未知盘式返回 nullptr
    static std::unique_ptr<Qimen> create(QimenType type);

    /// 排盘。
    /// @param dt    排盘时间
    /// @param ju    局数，0 表示自动定局；绝对值 1-9，符号表示阴阳（负数为阴遁）
    /// @param style 暗干排法
    /// @return 失败时返回 false，原因见 error()
    bool generate(const DateTime& dt, int ju = 0, HiddenStemStyle style = HiddenStemStyle::ByOriginalPalace);

    /// 最近一次失败的原因；不直接往 stderr 写，输出方式交给调用方决定
    const std::string& error() const
    {
        return error_;
    }

    /// 排盘结果
    const Chart& chart() const
    {
        return chart_;
    }

protected:
    /// 自动定局，需设置 chart_ 的 ju / yuan / yin_dun / solar_term
    virtual bool compute_ju(const DateTime& dt) = 0;

    /// 定局：手动指定局数时只补三元标记，否则调用 compute_ju()
    virtual bool resolve_ju(const DateTime& dt);

    /// 以下 8 个步骤按顺序执行，见 build()
    virtual void arrange_earth_stems() = 0;
    virtual void resolve_duty() = 0;
    virtual void arrange_stars() = 0;
    virtual void arrange_gates() = 0;
    virtual void arrange_deities() = 0;
    virtual void arrange_heaven_stems() = 0;
    virtual void resolve_void_and_horse() = 0;
    virtual void arrange_hidden_stems() = 0;

    /// 四柱，一次排盘内只计算一次
    const Pillars& pillars();

    /// 排盘时间
    const tyme::SolarTime& time() const
    {
        return *chart_.time;
    }

    Chart chart_{};
    HiddenStemStyle hidden_stem_style_{HiddenStemStyle::ByOriginalPalace};

private:
    void init_layout();
    bool prepare(const DateTime& dt, int ju);
    void build();

    std::optional<Pillars> pillars_{};
    std::string error_{};   ///< 失败原因
};

}   // namespace csp

#endif   // CSP_QIMEN_HPP
