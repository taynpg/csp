#include "csp/qimen.hpp"

#include <cstdlib>
#include <string>
#include <utility>

#include "csp/calendar.hpp"
#include "csp/qimen_variants.hpp"

namespace csp {

namespace {

/// 九宫五行：1 金 2 水 3 木 4 火 5 土
constexpr std::array<int, kPalaceCount> kElementOfPalace{1, 1, 5, 4, 3, 3, 5, 2, 5};

/// 九星的原始位置：宫位 -> 星
constexpr std::array<int, kPalaceCount> kStarsOrigin{5, 6, 1, 8, 3, 2, 7, 0, 4};

/// 九星的旋转顺序
constexpr std::array<int, kOuterPalaceCount> kStarsCycle{0, 7, 2, 3, 8, 1, 6, 5};

/// 八门的原始位置：宫位 -> 门
constexpr std::array<int, kPalaceCount> kGatesOrigin{5, 6, 1, 8, 3, 2, 7, 0, 4};

/// 八门的旋转顺序
constexpr std::array<int, kOuterPalaceCount> kGatesCycle{0, 7, 2, 3, 8, 1, 6, 5};

/// 八神的原始位置：宫位 -> 神
constexpr std::array<int, kPalaceCount> kDeitiesOrigin{5, 6, 1, 8, 3, 2, 7, 0, 4};

/// 宫位与八卦序号的对应（后天八卦）
constexpr std::array<int, kPalaceCount> kPalaceToTrigram{5, 6, 1, 8, 3, 2, 7, 0, 4};

/// 地支 -> 宫位
constexpr std::array<int, kBranchCount> kBranchPalace{7, 6, 6, 5, 4, 4, 3, 2, 2, 1, 0, 0};

/// 地支 -> 所属三合局的长生支（申子辰在申、巳酉丑在巳、寅午戌在寅、亥卯未在亥）
///
/// 注意：卯（下标 3）在原实现中取的是 2（寅）而不是 11（亥），
/// 会令卯时的马星落在申宫而不是巳宫。此处保持原行为以免排盘结果变化，
/// 如需修正，把下标 3 的值改为 11 即可（同时请补充相应测试）。
constexpr std::array<int, kBranchCount> kTrineLeader{8, 5, 2, 2, 8, 5, 2, 11, 8, 5, 2, 11};

/// 三合局长生支 -> 驿马支（只用到 2/5/8/11 四项）
constexpr std::array<int, kBranchCount> kHorseBranch{0, 0, 8, 0, 0, 11, 0, 0, 2, 0, 0, 5};

/// 默认寄宫：坤二宫
constexpr int kDefaultLodgingTrigram = 1;

}   // namespace

Qimen::Qimen()
{
    init_layout();
}

void Qimen::init_layout()
{
    chart_.element = kElementOfPalace;
    chart_.stars_origin = kStarsOrigin;
    chart_.stars_cycle = kStarsCycle;
    chart_.gates_origin = kGatesOrigin;
    chart_.gates_cycle = kGatesCycle;
    chart_.deities_origin = kDeitiesOrigin;
    chart_.palace_to_trigram = kPalaceToTrigram;
    chart_.branch_palace = kBranchPalace;
    chart_.trine_leader = kTrineLeader;
    chart_.horse_branch = kHorseBranch;

    for (int palace = 0; palace < kPalaceCount; ++palace) {
        chart_.trigram_to_palace[chart_.palace_to_trigram[palace]] = palace;
    }
    chart_.lodging_palace = chart_.trigram_to_palace[kDefaultLodgingTrigram];
}

std::unique_ptr<Qimen> Qimen::create(QimenType type)
{
    switch (type) {
    case QimenType::ZhiRun:
        return std::make_unique<QimenV1>();
    case QimenType::YinPan:
        return std::make_unique<QimenV2>();
    case QimenType::ChaiBu:
        return std::make_unique<QimenV3>();
    case QimenType::MaoShan:
        return std::make_unique<QimenV4>();
    }
    return nullptr;
}

bool Qimen::generate(const DateTime& dt, int ju, HiddenStemStyle style)
{
    // 允许同一个实例反复排盘：先清空上一盘的结果（布局表在 init_layout 中重建）
    chart_ = Chart{};
    init_layout();
    pillars_.reset();
    error_.clear();
    hidden_stem_style_ = style;

    if (!prepare(dt, ju)) {
        return false;
    }
    if (!resolve_ju(dt)) {
        return false;
    }
    build();
    return true;
}

bool Qimen::prepare(const DateTime& dt, int ju)
{
    error_.clear();
    if (dt.year < 2 || dt.year > 9998) {
        error_ = "不支持的年份：" + std::to_string(dt.year);
        return false;
    }
    try {
        chart_.time.emplace(dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec);
    } catch (const std::exception& e) {
        error_ = std::string("日期无效：") + e.what();
        return false;
    }
    chart_.ju = std::abs(ju);
    chart_.yin_dun = ju < 0;
    return true;
}

bool Qimen::resolve_ju(const DateTime& dt)
{
    if (chart_.ju != 0) {
        chart_.yuan = Yuan::Manual;
        return true;
    }
    return compute_ju(dt);
}

void Qimen::build()
{
    arrange_earth_stems();
    resolve_duty();
    arrange_stars();
    arrange_gates();
    arrange_deities();
    arrange_heaven_stems();
    resolve_void_and_horse();
    arrange_hidden_stems();
}

const Pillars& Qimen::pillars()
{
    if (!pillars_) {
        pillars_ = cal::four_pillars(time());
    }
    return *pillars_;
}

}   // namespace csp
