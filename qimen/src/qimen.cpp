#include "qimen.h"
#include "qm_v1.h"
#include "qm_v2.h"
#include "qm_v3.h"
#include "qm_v4.h"
#include <chrono>
#include <iomanip>
#include <iostream>

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

    data_.sanhe_[0] = 8;
    data_.sanhe_[1] = 5;
    data_.sanhe_[2] = 2;
    data_.sanhe_[3] = 2;
    data_.sanhe_[4] = 8;
    data_.sanhe_[5] = 5;
    data_.sanhe_[6] = 2;
    data_.sanhe_[7] = 11;
    data_.sanhe_[8] = 8;
    data_.sanhe_[9] = 5;
    data_.sanhe_[10] = 2;
    data_.sanhe_[11] = 11;

    data_.dzcong_[8] = 2;
    data_.dzcong_[5] = 11;
    data_.dzcong_[2] = 8;
    data_.dzcong_[2] = 8;
    data_.dzcong_[11] = 5;
}

int Qimen::mod(int base, int value)
{
    return (value % base + base) % base;
}

DateTime Qimen::now_time()
{
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&now_time_t);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    DateTime result;
    result.year = static_cast<int>(local_tm.tm_year + 1900);
    result.mon = static_cast<int>(local_tm.tm_mon + 1);
    result.day = static_cast<int>(local_tm.tm_mday);
    result.hour = static_cast<int>(local_tm.tm_hour);
    result.min = static_cast<int>(local_tm.tm_min);
    result.sec = static_cast<int>(local_tm.tm_sec);
    result.msec = static_cast<int>(now_ms.count());
    return result;
}

int Qimen::get_jiazi_index(int t, int d)
{
    int n = d;
    for (int i = 0; i < 5; ++i) {
        if ((n % 10) == t) {
            break;
        }
        n += 12;
    }
    return n;
}
int Qimen::get_index(const int* d, int size, int value)
{
    if (!d) {
        return -1;
    }

    int n = -1;
    for (int i = 0; i < size; ++i) {
        if (d[i] == value) {
            n = i;
            break;
        }
    }
    return n;
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

bool Qimen::generate(const DateTime& dt, int ju)
{
    return false;
}

bool Qimen::set_and_check(const DateTime& dt, int ju)
{
    try {
        data_.dt_ = std::make_shared<tyme::SolarTime>(dt.year, dt.mon, dt.day, dt.hour, dt.min, dt.sec);
        data_.ju = std::abs(ju);
        data_.is_yin = ju < 0 ? true : false;
        return true;
    } catch (const std::exception& e) {
        std::cout << "Invalid Datetime :" << e.what() << std::endl;
        return false;
    }
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

void Qimen::set_jigong(int gong)
{
    // 这里减 1 是取索引
    int n = gong - 1;
    // 这里要把寄宫转成位置
    data_.jigong = data_.gua2pos[n];
}

void Qimen::set_dizhi()
{
    data_.zhi[0] = 7;
    data_.zhi[1] = 6;
    data_.zhi[2] = 6;
    data_.zhi[3] = 5;
    data_.zhi[4] = 4;
    data_.zhi[5] = 4;
    data_.zhi[6] = 3;
    data_.zhi[7] = 2;
    data_.zhi[8] = 2;
    data_.zhi[9] = 1;
    data_.zhi[10] = 0;
    data_.zhi[11] = 0;
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

QimenData Qimen::get_result() const
{
    return data_;
}

DateTime Qimen::solar(const tyme::SolarTime& dt)
{
    DateTime re;
    re.year = dt.get_year();
    re.mon = dt.get_month();
    re.day = dt.get_day();
    re.hour = dt.get_hour();
    re.min = dt.get_minute();
    re.sec = dt.get_second();
    return re;
}
DateTime Qimen::lunar(const tyme::SolarTime& dt)
{
    DateTime re;
    re.year = dt.get_lunar_hour().get_year();
    re.mon = dt.get_lunar_hour().get_month();
    re.day = dt.get_lunar_hour().get_day();
    re.hour = dt.get_lunar_hour().get_hour();
    re.min = dt.get_lunar_hour().get_minute();
    re.sec = dt.get_lunar_hour().get_second();
    return re;
}
JiaZi Qimen::jiazi(const tyme::SolarTime& dt)
{
    JiaZi re;
    re.yi = dt.get_sixty_cycle_hour()
                .get_sixty_cycle_day()
                .get_sixty_cycle_month()
                .get_sixty_cycle_year()
                .get_sixty_cycle()
                .get_index();
    re.mi = dt.get_sixty_cycle_hour().get_sixty_cycle_day().get_sixty_cycle_month().get_sixty_cycle().get_index();
    re.di = dt.get_sixty_cycle_hour().get_sixty_cycle_day().get_sixty_cycle().get_index();
    re.hi = dt.get_sixty_cycle_hour().get_sixty_cycle().get_index();

    return re;
}

tyme::SolarTime Qimen::get_jq(const tyme::SolarTime& dt, int& index, bool is_sec)
{
    int mon = dt.get_month();
    int year = dt.get_year();
    index = (is_sec ? mon * 2 : mon * 2 - 1);
    if (index == 24) {
        year += 1;
        index = 0;
    }
    auto term = tyme::SolarTerm::from_index(year, index);
    return term.get_julian_day().get_solar_time();
}

DateTime::DateTime(int y, int m, int d, int h, int mi, int s)
{
    year = y;
    mon = m;
    day = d;
    hour = h;
    min = mi;
    sec = s;
}

}   // namespace csp