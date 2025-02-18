#include "qimen.h"
#include "qm_v1.h"
#include "qm_v2.h"
#include "qm_v3.h"
#include "qm_v4.h"
#include <cmath>
#include <cstdio>

namespace csp {

Qimen::Qimen()
{
    set_wuxing();
    set_jiuxing_pre();
    set_jiuxing_turn();
    set_gong_pos();
    set_bamen_pre();
    set_bamen_turn();
    set_bashen_pre();
    set_dizhi();
    // 默认寄宫寄 坤二宫
    set_jigong(2);
}

CSPT Qimen::get_jiazi_index(CSPT t, CSPT d)
{
    return CSPT();
}

CSPT Qimen::get_index(const CSPT* d, CSPT size, CSPT value)
{
    return CSPT();
}

std::shared_ptr<Qimen> Qimen::instance(QimenType type)
{
    std::shared_ptr<Qimen> re = nullptr;
    switch (type) {
    case QimenType::QM_TYPE_SJ_ZHIRUN:
        re = std::make_shared<QimenV1>();
        break;
    case QimenType::QM_TYPE_SJ_YIN:
        re = std::make_shared<QimenV2>();
        break;
    case QimenType::QM_TYPE_SJ_CHAIBU:
        re = std::make_shared<QimenV3>();
        break;
    case QimenType::QM_TYPE_SJ_MAOSHAN:
        re = std::make_shared<QimenV4>();
        break;
    default:
        break;
    }
    return re;
}

const char* Qimen::get_error() const
{
    return err_;
}

bool Qimen::generate(const DateTime& dt, int ju, CalendarType ct)
{
    return false;
}

bool Qimen::set_and_check(const DateTime& dt, int ju, CalendarType ct)
{
    ctype_ = ct;
    cal_ = Calendar::instance(ctype_);
    if (!cal_->check_format_only(dt)) {
        std::snprintf(err_, sizeof(err_), "%s", "日期不在支持的范围内。");
        return false;
    }
    if (ju < -9 || ju > 9) {
        std::snprintf(err_, sizeof(err_), "%s", "局数设定不在[-9, 9]范围内。");
        return false;
    }
    dt_ = dt;
    data_.ju = std::abs(ju);
    data_.is_yin = (ju < 0 ? true : false);
    return true;
}

void Qimen::set_wuxing()
{
    data_.wuxing[0] = 1;
    data_.wuxing[1] = 1;
    data_.wuxing[2] = 5;
    data_.wuxing[3] = 4;
    data_.wuxing[4] = 3;
    data_.wuxing[5] = 3;
    data_.wuxing[6] = 5;
    data_.wuxing[7] = 2;
    data_.wuxing[8] = 5;
}

void Qimen::set_jiuxing_pre()
{
    data_.jiuxp[0] = 5;
    data_.jiuxp[1] = 6;
    data_.jiuxp[2] = 1;
    data_.jiuxp[3] = 8;
    data_.jiuxp[4] = 3;
    data_.jiuxp[5] = 2;
    data_.jiuxp[6] = 7;
    data_.jiuxp[7] = 0;
    data_.jiuxp[8] = 4;
}

void Qimen::set_jiuxing_turn()
{
    data_.jiuxt[0] = 0;
    data_.jiuxt[1] = 7;
    data_.jiuxt[2] = 2;
    data_.jiuxt[3] = 3;
    data_.jiuxt[4] = 8;
    data_.jiuxt[5] = 1;
    data_.jiuxt[6] = 6;
    data_.jiuxt[7] = 5;
}

void Qimen::set_gong_pos()
{
    // 定义位置从 0 开始分别对应卦序
    data_.pos2gua[0] = 5;
    data_.pos2gua[1] = 6;
    data_.pos2gua[2] = 1;
    data_.pos2gua[3] = 8;
    data_.pos2gua[4] = 3;
    data_.pos2gua[5] = 2;
    data_.pos2gua[6] = 7;
    data_.pos2gua[7] = 0;
    data_.pos2gua[8] = 4;

    // gua2pos_ 就是后天八卦的顺序数和定义位置的对照。
    for (int i = 0; i < 9; ++i) {
        data_.gua2pos[data_.pos2gua[i]] = i;
    }
}

void Qimen::set_bamen_pre()
{
    data_.bamenp[0] = 5;
    data_.bamenp[1] = 6;
    data_.bamenp[2] = 1;
    data_.bamenp[3] = 8;
    data_.bamenp[4] = 3;
    data_.bamenp[5] = 2;
    data_.bamenp[6] = 7;
    data_.bamenp[7] = 0;
    data_.bamenp[8] = 4;
}

void Qimen::set_bamen_turn()
{
    data_.bament[0] = 0;
    data_.bament[1] = 7;
    data_.bament[2] = 2;
    data_.bament[3] = 3;
    data_.bament[4] = 8;
    data_.bament[5] = 1;
    data_.bament[6] = 6;
    data_.bament[7] = 5;
}

void Qimen::set_bashen_pre()
{
    data_.bashenp[0] = 5;
    data_.bashenp[1] = 6;
    data_.bashenp[2] = 1;
    data_.bashenp[3] = 8;
    data_.bashenp[4] = 3;
    data_.bashenp[5] = 2;
    data_.bashenp[6] = 7;
    data_.bashenp[7] = 0;
    data_.bashenp[8] = 4;
}

void Qimen::set_jigong(CSPT gong)
{
    // 这里减 1 是取索引
    int n = gong - 1;
    // 这里要把寄宫转成位置
    data_.jigong = gua2pos_[n];
}

void Qimen::set_dizhi()
{
    zhi_[0] = 7;
    zhi_[1] = 6;
    zhi_[2] = 6;
    zhi_[3] = 5;
    zhi_[4] = 4;
    zhi_[5] = 4;
    zhi_[6] = 3;
    zhi_[7] = 2;
    zhi_[8] = 2;
    zhi_[9] = 1;
    zhi_[10] = 0;
    zhi_[11] = 0;
}

std::shared_ptr<Calendar> Qimen::get_cal() const
{
    return cal_;
}

QimenData Qimen::get_result() const
{
    return data_;
}

}   // namespace csp
