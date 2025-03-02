#include "qm_v1.h"
#include <cmath>

namespace csp {
QimenV1::QimenV1()
{
    // 下标 0 指小寒，即从小寒开始向后排列。
    juqi_[0] = 2851;
    juqi_[1] = 3961;
    juqi_[2] = 8521;
    juqi_[3] = 9631;
    juqi_[4] = 1741;
    juqi_[5] = 3961;
    juqi_[6] = 4171;
    juqi_[7] = 5281;
    juqi_[8] = 4171;
    juqi_[9] = 5281;
    juqi_[10] = 6391;
    juqi_[11] = 9360;
    juqi_[12] = 8250;
    juqi_[13] = 7140;
    juqi_[14] = 2580;
    juqi_[15] = 1470;
    juqi_[16] = 9360;
    juqi_[17] = 7140;
    juqi_[18] = 6930;
    juqi_[19] = 5820;
    juqi_[20] = 6930;
    juqi_[21] = 5820;
    juqi_[22] = 4710;
    juqi_[23] = 1741;

    wby_[0] = 6;
    wby_[1] = 17;
    wby_[2] = 28;
    wby_[3] = 39;
    wby_[4] = 50;
    wby_[5] = 1;
    wby_[6] = 12;
    wby_[7] = 33;
    wby_[8] = 44;
    wby_[9] = 55;
}

bool QimenV1::generate(const DateTime& dt, int ju, CalendarType ct)
{
    if (!Qimen::set_and_check(dt, ju, ct)) {
        return false;
    }
    if (!cal_->generate(dt_)) {
        return false;
    }
    if (data_.ju == 0 && !cal_ju(dt)) {
        return false;
    }

    cal_dipan();
    cal_duty();
    cal_jiuxing();
    cal_bamen();
    cal_bashen();
    cal_tianpan();
    cal_other();

    return true;
}

void QimenV1::cal_dipan()
{
    // 所谓的几局，就是甲子戊居于第几宫
    int s = Calendar::mod(9, data_.ju - 1);
    data_.dp[data_.gua2pos[s]] = 4;
    int nk = data_.is_yin ? -1 : 1;

    for (int i = 0; i < 5; ++i) {
        data_.dp[data_.gua2pos[Calendar::mod(9, s += nk)]] = 5 + i;
    }
    for (int i = 0; i < 3; ++i) {
        data_.dp[data_.gua2pos[Calendar::mod(9, s += nk)]] = 3 - i;
    }

    // 判断是否是 五不遇时
    int hg = cal_->get_ganzhi().hg;
    int hz = cal_->get_ganzhi().hz;
    int i = Qimen::get_jiazi_index(hg, hz);
    if (wby_[cal_->get_ganzhi().dg] == i) {
        data_.wubuyu = true;
    } else {
        data_.wubuyu = false;
    }
}

void QimenV1::cal_duty()
{
    // 六十甲子
    int hi = Qimen::get_jiazi_index(cal_->get_ganzhi().hg, cal_->get_ganzhi().hz);
    // 查看旬头遁甲位置
    int gi = (hi / 10) + 4;
    // 这里算出来的值是场地位置
    data_.duty = get_index(data_.dp, 9, gi);
}

void QimenV1::cal_jiuxing()
{
    // 先查验当前的局是否为伏吟
    if (cal_->get_ganzhi().hg == 0) {
        for (int i = 0; i < 9; ++i) {
            data_.jiuxr[i] = data_.jiuxp[i];
            data_.bamenr[i] = data_.bamenp[i];
        }
        return;
    }
    // 时辰天干在地盘的位置
    int s = get_index(data_.dp, 9, cal_->get_ganzhi().hg);
    // 如果这个位置在中宫，那么从寄宫开始
    s = (s == 8 ? data_.jigong : s);

    int nx = data_.jiuxp[data_.duty];
    // 如果当前的值符为天禽星，那么也归为原寄宫星
    nx = (nx == 4 ? data_.jiuxp[data_.jigong] : nx);
    data_.jiuxr[s] = nx;

    // 查找当前的九星的起始相对位置
    int nt = get_index(data_.jiuxt, 8, nx);
    for (int i = 0; i < 8; ++i) {
        data_.jiuxr[Calendar::mod(8, --s)] = data_.jiuxt[Calendar::mod(8, ++nt)];
    }
}

void QimenV1::cal_bamen()
{
    // 查看当前旬头六仪
    int hi = Qimen::get_jiazi_index(cal_->get_ganzhi().hg, cal_->get_ganzhi().hz);
    // 查看当前时辰距离符头有几跳
    int dif = hi - (hi / 10) * 10;

    int nk = data_.is_yin ? -1 : 1;

    /*
        bm_pre_ 中仅有 8 个数据，当值符值使位置在中宫8的时候，转成寄宫
        但是转的时候不从寄宫开始转
    */
    int s = data_.duty;
    s = (s == 8 ? data_.jigong : s);

    int gi = data_.pos2gua[data_.duty];
    for (int i = 0; i < dif; ++i) {
        gi = Calendar::mod(9, gi += nk);
    }
    int np = data_.gua2pos[gi];
    // 如果门转到了中宫，也是按照寄宫处理。
    np = (np == 8 ? data_.jigong : np);

    int mi = get_index(data_.bament, 8, data_.bamenp[s]);
    for (int i = 0; i < 8; ++i, --np, ++mi) {
        data_.bamenr[Calendar::mod(8, np)] = data_.bament[Calendar::mod(8, mi)];
    }
}

void QimenV1::cal_bashen()
{
    int nx = data_.jiuxp[data_.duty];
    nx = (nx == 4 ? data_.jiuxp[data_.jigong] : nx);
    // 看看值符被转到哪里了
    int s = get_index(data_.jiuxr, 9, nx);
    int nk = data_.is_yin ? -1 : 1;
    for (int i = 0; i < 9; ++i, s += nk) {
        if (i == 4) {
            ++i;
        }
        data_.bashenr[Calendar::mod(8, s)] = i;
    }
}

void QimenV1::cal_tianpan()
{
    // 当前九星宫位的天盘就是：当前星原来宫位中的地盘
    for (int i = 0; i < 8; ++i) {
        data_.tp[i] = data_.dp[data_.gua2pos[data_.jiuxr[i]]];
    }
}

void QimenV1::cal_other()
{
    auto gen = [this](CSPT g, CSPT z, CSPT& a, CSPT& b) {
        int j = Qimen::get_jiazi_index(g, z);
        const auto& r = cal_xunkong(j);
        a = r.first;
        b = r.second;
    };

    gen(cal_->get_ganzhi().yg, cal_->get_ganzhi().yz, data_.xunkong[0], data_.xunkong[1]);
    gen(cal_->get_ganzhi().mg, cal_->get_ganzhi().mz, data_.xunkong[2], data_.xunkong[3]);
    gen(cal_->get_ganzhi().dg, cal_->get_ganzhi().dz, data_.xunkong[4], data_.xunkong[5]);
    gen(cal_->get_ganzhi().hg, cal_->get_ganzhi().hz, data_.xunkong[6], data_.xunkong[7]);

    data_.kongw[0] = data_.zhi[data_.xunkong[6]];
    data_.kongw[1] = data_.zhi[data_.xunkong[7]];
    data_.maxing = data_.zhi[dzcong_[sanhe_[cal_->get_ganzhi().hz]]];
}

bool QimenV1::inference()
{
    year_datas_.clear();

    CSPT start = 22;
    CSPT dif = 0;
    CSPT jiazi = 0;
    CSPT tem = 0;

    DateTime pdx(dt_.year - 1, 12, 1, 12, 0, 0);
    DateTime dx(dt_.year, 12, 1, 12, 0, 0);
    DateTime mz(dt_.year, 6, 6, 12, 0, 0);
    DateTime dt{};

    DateTime t;
    DateTime c;

    if (!cur_month_jq(pdx, t, jiazi, dif)) {
        return false;
    }
    c = t;
    // 保存大雪的三元
    save_part(c, start, jiazi, -1, 15 - dif);

    if (dif < 9) {
        start = Calendar::mod(24, ++start);
    }

    // 保存大雪到芒种
    save_part(c, start, jiazi, 10, 300);
    if (!cur_month_jq(mz, t, tem, dif)) {
        return false;
    }

    dt = c;
    cal_->pre_day(dt, 1);
    dif = std::abs(cal_->days_difference(dt, t));
    save_part(c, start, jiazi, -1, 14);
    if (dif < 9) {
        start = Calendar::mod(24, ++start);
    }

    save_part(c, start, jiazi, 22, 300);
    if (!cur_month_jq(dx, t, tem, dif)) {
        return false;
    }

    dt = c;
    cal_->pre_day(dt, 1);
    dif = std::abs(cal_->days_difference(dt, t));
    save_part(c, start, jiazi, -1, 14);
    if (dif < 9) {
        start = Calendar::mod(24, ++start);
    }
    save_part(c, start, jiazi, -1, 60);
    return true;
}

bool QimenV1::search_day(const DateTime& datetime, OneDay& o)
{
    bool find = false;
    for (const auto& item : year_datas_) {
        if (Calendar::is_same_day(item.dt, datetime)) {
            o = item;
            find = true;
            break;
        }
    }
    return find;
}

bool QimenV1::cur_month_jq(const DateTime& dt, DateTime& jie, CSPT& jiazi, CSPT& dif)
{
    auto tcal = Calendar::instance(ctype_);
    if (!tcal->generate(dt)) {
        return false;
    }
    auto jqs = tcal->get_jq_info();
    jie = jqs[2].dt;

    if (!tcal->generate(jie)) {
        return false;
    }
    jiazi = get_jiazi_index(tcal->get_ganzhi().dg, tcal->get_ganzhi().dz);
    CSPT ft = (jiazi / 15) * 15;
    dif = jiazi - ft;
    if (dif < 0) {
        dif = -dif;
    }
    return true;
}

void QimenV1::save_part(DateTime& sdt, CSPT& upper, CSPT& jiazi, CSPT pur, CSPT days)
{
    int cy = 0;
    upper = Calendar::mod(24, upper);
    for (int i = 0; i < days; ++i, ++cy) {
        year_datas_.emplace_back(OneDay{sdt, upper, jiazi});
        cal_->next_day(sdt, 1);
        jiazi = Calendar::mod(60, ++jiazi);
        if (cy == 15) {
            upper = Calendar::mod(24, ++upper);
            if (upper == pur) {
                break;
            }
            cy = 0;
        }
    }
}

std::pair<int, int> QimenV1::cal_xunkong(CSPT jiazi)
{
    int ka = ((jiazi / 10) * 10 + 10) % 12;
    int kb = ((jiazi / 10) * 10 + 11) % 12;
    return std::make_pair(ka, kb);
}

bool QimenV1::cal_ju(const DateTime& dt)
{
    if (data_.ju != 0) {
        data_.yuan = 0;
        return true;
    }

    inference();
    OneDay o;
    if (!search_day(dt, o)) {
        return false;
    }

    data_.jiazi = o.ganzhi;
    data_.jieq = o.jq;

    int tv = juqi_[o.jq];
    if ((tv % 10) == 1) {
        data_.is_yin = false;
    } else {
        data_.is_yin = true;
    }

    int tv2 = (o.ganzhi % 15) / 5;
    if (tv2 == 0) {
        data_.yuan = 3;
        data_.ju = tv / 1000;
    } else if (tv2 == 1) {
        data_.yuan = 2;
        data_.ju = (tv / 100) % 10;
    } else {
        data_.yuan = 1;
        data_.ju = (tv / 10) % 10;
    }
    return true;
}

OneDay::OneDay(const DateTime& adt, CSPT ajq, CSPT agz)
{
    dt = adt;
    jq = ajq;
    ganzhi = agz;
}
}   // namespace csp