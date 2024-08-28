#include "qm_v1.h"

#include <cmath>

namespace cppbox {

CQiMenV1::CQiMenV1()
{
    pcal_ = nullptr;
    jushu_ = 0;
    cal_type_ = CalendarType::CALENDAR_V1;

    // 下标 0 指小寒，即从小寒开始向后排列。
    ju_qi_[0] = 2851;
    ju_qi_[1] = 3961;
    ju_qi_[2] = 8521;
    ju_qi_[3] = 9631;
    ju_qi_[4] = 1741;
    ju_qi_[5] = 3961;
    ju_qi_[6] = 4171;
    ju_qi_[7] = 5281;
    ju_qi_[8] = 4171;
    ju_qi_[9] = 5281;
    ju_qi_[10] = 6391;
    ju_qi_[11] = 9360;
    ju_qi_[12] = 8250;
    ju_qi_[13] = 7140;
    ju_qi_[14] = 2580;
    ju_qi_[15] = 1470;
    ju_qi_[16] = 9360;
    ju_qi_[17] = 7140;
    ju_qi_[18] = 6930;
    ju_qi_[19] = 5820;
    ju_qi_[20] = 6930;
    ju_qi_[21] = 5820;
    ju_qi_[22] = 4710;
    ju_qi_[23] = 1741;

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

CQiMenV1::~CQiMenV1()
{
    if (!one_year_.empty()) {
        std::vector<OneDay*>::iterator it;
        for (it = one_year_.begin(); it != one_year_.end(); ++it) {
            delete *it;
        }
    }
}

// 传入日期数据
bool CQiMenV1::run(const QiParam& info, CalendarType type)
{
    if (!base_run(info, type)) {
        return false;
    }

    if (info.ju_ == 0) {
        inference();
        // printYearDay();

        OneDay* pToday = search_day(info.datetime_);
        if (!pToday) {
            return false;
        }

        jz_ = pToday->gz_index_;
        jq_ = pToday->jie_index_;

        int nResult = ju_qi_[pToday->jie_index_];
        if ((nResult % 10) == 1) {
            is_yin_ = false;
        } else {
            is_yin_ = true;
        }

        int num = (pToday->gz_index_ % 15) / 5;
        if (num == 0) {
            yuan_ = 3;
            jushu_ = nResult / 1000;
        } else if (num == 1) {
            yuan_ = 2;
            jushu_ = (nResult / 100) % 10;
        } else {
            yuan_ = 1;
            jushu_ = (nResult / 10) % 10;
        }
    } else {
        jushu_ = std::abs(info.ju_);
        if ((jushu_ - 9) > 0) {
            return false;
        }
        if (info.ju_ < 0) {
            is_yin_ = true;
        } else {
            is_yin_ = false;
        }
        yuan_ = 0;
    }

    // 这里跑一下今天的日期以供后续使用
    pcal_->set_datetime(datetime_);

    gen_dipan();
    gen_zhi();
    gen_jx();
    gen_bm();
    gen_bs();
    gen_tp();
    gen_other();

    return true;
}

void CQiMenV1::get_cur_jie(const CDateTime& datetime, CDateTime& datetimeJie, int& nJiazi, int& nFutouDiff)
{
    pcal_->set_datetime(datetime);
    datetimeJie = pcal_->get_jie().jq[2].dt_;
    pcal_->set_datetime(datetimeJie);
    nJiazi = get_jz(pcal_->get_sz().dg_, pcal_->get_sz().dz_);
    int nFutou = (nJiazi / 15) * 15;
    nFutouDiff = nJiazi - nFutou;
    if (nFutouDiff < 0) {
        nFutouDiff = -nFutouDiff;
    }
}

// 推算一年的日历
void CQiMenV1::inference()
{
    int nTem = 0;
    int nStartJie = 22;
    int nJiazi = 0;
    int nFutouDiff = 0;

    CDateTime dateDaxuePreYear(datetime_.date_.year_ - 1, 12, 1, 12, 0, 0);
    CDateTime dateDaxue(datetime_.date_.year_, 12, 1, 12, 0, 0);
    CDateTime dateMangzhong(datetime_.date_.year_, 6, 6, 12, 0, 0);
    CDateTime dateTem;

    CDateTime dateJieTime;
    CDateTime cycleDatetime;
    get_cur_jie(dateDaxuePreYear, dateJieTime, nJiazi, nFutouDiff);
    cycleDatetime = dateJieTime;
    // 保存大雪的三元
    save_part(cycleDatetime.date_, nStartJie, nJiazi, -1, 15 - nFutouDiff);
    if (nFutouDiff < 9) {
        nStartJie = cb::remain(24, ++nStartJie);
    }
    // 保存大雪到芒种
    save_part(cycleDatetime.date_, nStartJie, nJiazi, 10, 300);
    get_cur_jie(dateMangzhong, dateJieTime, nTem, nFutouDiff);

    // 递增日期此时存的是最后一个日期的下一个日期，所以这里算差值要回退一天
    dateTem = CDateTime(cycleDatetime.date_);
    pcal_->pre(dateTem);
    int nDayDiff = pcal_->get_diff_day(dateTem.date_, dateJieTime.date_);
    nDayDiff = nDayDiff < 0 ? -nDayDiff : nDayDiff;
    // 保存芒种三元
    save_part(cycleDatetime.date_, nStartJie, nJiazi, -1, 14);
    if (nDayDiff < 9) {
        nStartJie = cb::remain(24, ++nStartJie);
    }
    // 保存芒种到大雪
    save_part(cycleDatetime.date_, nStartJie, nJiazi, 22, 300);
    get_cur_jie(dateDaxue, dateJieTime, nTem, nFutouDiff);
    dateTem = CDateTime(cycleDatetime.date_);
    // 递增日期此时存的是最后一个日期的下一个日期，所以这里算差值要回退一天
    pcal_->pre(dateTem);
    nDayDiff = pcal_->get_diff_day(dateTem.date_, dateJieTime.date_);
    nDayDiff = nDayDiff < 0 ? -nDayDiff : nDayDiff;
    // 保存大雪的三元
    save_part(cycleDatetime.date_, nStartJie, nJiazi, -1, 14);
    if (nDayDiff < 9) {
        nStartJie = cb::remain(24, ++nStartJie);
    }
    // 保存大雪到立春
    save_part(cycleDatetime.date_, nStartJie, nJiazi, -1, 60);
}

void CQiMenV1::save_part(CDate& date, int& nUpper, int& nJiazi, int nPur, int nDays)
{
    int nCycle = 0;
    nUpper = cb::remain(24, nUpper);
    for (int i = 0; i < nDays; ++i, ++nCycle) {
        if (nCycle == 15) {
            nUpper = cb::remain(24, ++nUpper);
            if (nUpper == nPur) {
                save_day(date, nUpper, nJiazi);
                pcal_->next(date);
                nJiazi = cb::remain(60, ++nJiazi);
                break;
            }
            nCycle = 0;
        }
        save_day(date, nUpper, nJiazi);
        pcal_->next(date);
        nJiazi = cb::remain(60, ++nJiazi);
    }
}

void CQiMenV1::save_day(const CDate& date, int nJie, int& nJiazi)
{
    auto* pDay = new OneDay();
    pDay->gz_index_ = nJiazi;
    pDay->jie_index_ = nJie;
    pDay->date = date;
    one_year_.push_back(pDay);
}

// 在一年的日历中查找当天的信息
OneDay* CQiMenV1::search_day(const CDateTime& datetime)
{
    OneDay* p = nullptr;
    std::vector<OneDay*>::const_iterator it;
    for (it = one_year_.begin(); it != one_year_.end(); ++it) {
        if ((*it)->date == datetime.date_) {
            p = *it;
            break;
        }
    }
    return p;
}

// 打印所推演的一年日历
/*void CQiMenV1::printYearDay() {

    std::vector<OneDay *>::const_iterator it;
    for (it = m_pOneYear.begin(); it != m_pOneYear.end(); ++it) {

        std::printf("%04d-%02d-%02d %02d %02d\n" , (*it)->date.m_nYear,
(*it)->date.mon_,
        (*it)->date.day_, (*it)->nJieIndex, (*it)->nGanZhiIndex);
    }
}*/

// 获取给定甲子的两个寻空
void CQiMenV1::get_xk(int nJiazi, int& nKongA, int& nKongB)
{
    nKongA = ((nJiazi / 10) * 10 + 10) % 12;
    nKongB = ((nJiazi / 10) * 10 + 11) % 12;
}

// 排地盘
void CQiMenV1::gen_dipan()
{
    // 所谓的几局，就是甲子戊居于第几宫
    int nStartIndex = cb::remain(9, jushu_ - 1);
    // 转成定义位置
    int nPosition = gua2pos_[nStartIndex];
    // 甲子戊
    d_[nPosition] = 4;
    int nk{};
    if (is_yin_) {
        nk = -1;
    } else {
        nk = 1;
    }
    for (int i = 0; i < 5; ++i) {
        d_[gua2pos_[cb::remain(9, nStartIndex += nk)]] = 5 + i;
    }
    for (int i = 0; i < 3; ++i) {
        d_[gua2pos_[cb::remain(9, nStartIndex += nk)]] = 3 - i;
    }

    // 判断是否是 五不遇时
    int hg = pcal_->get_sz().hg_;
    int hz = pcal_->get_sz().hz_;
    if (wby_[pcal_->get_sz().dg_] == CQimen::get_jz(hg, hz)) {
        wbys_ = true;
    } else {
        wbys_ = false;
    }
}
// 查找值符，值使
void CQiMenV1::gen_zhi()
{
    // 六十甲子
    int hindex = get_jz(pcal_->get_sz().hg_, pcal_->get_sz().hz_);
    // 查看旬头遁甲位置
    int ganIndex = (hindex / 10) + 4;
    // 这里根据需要搜寻所在地盘的宫位在哪里
    // 这里算出来的值是场地位置
    zf_ = get_index(d_, 9, ganIndex);
    zs_ = zf_;
}
// 排九星
void CQiMenV1::gen_jx()
{
    // 先查验当前的局是否为伏吟
    if (pcal_->get_sz().hg_ == 0) {
        for (int i = 0; i < 9; ++i) {
            jx_re_[i] = jx_pre_[i];
            bm_re_[i] = bm_pre_[i];
        }
        return;
    }
    // 时辰天干在地盘的位置
    int nstart = get_index(d_, 9, pcal_->get_sz().hg_);
    // 如果这个位置在中宫，那么从寄宫开始
    if (nstart == 8) {
        nstart = jg_;
    }
    int nxing = jx_pre_[zf_];
    // 如果当前的值符为天禽星，那么也归为原寄宫星
    if (nxing == 4) {
        nxing = jx_pre_[jg_];
    }
    jx_re_[nstart] = nxing;
    // 查找当前的九星的起始相对位置
    int nturn = get_index(jx_turn_, 8, nxing);
    for (int i = 0; i < 8; ++i) {
        jx_re_[cb::remain(8, --nstart)] = jx_turn_[cb::remain(8, ++nturn)];
    }
}
// 排八门
void CQiMenV1::gen_bm()
{
    // 查看当前旬头六仪
    int nJiazi = get_jz(pcal_->get_sz().hg_, pcal_->get_sz().hz_);
    // 查看当前时辰距离符头有几跳
    int ndiff = nJiazi - (nJiazi / 10) * 10;

    int nk = 1;
    if (is_yin_) {
        nk = -1;
    }

    // bm_pre_ 中仅有 8 个数据，当值符值使位置在中宫8的时候，转成寄宫
    // 但是转的时候不从寄宫开始转
    int startPos = zs_;
    if (startPos == 8) {
        startPos = jg_;
    }

    // pos2gua_ 用于位置转卦序
    int guaIndex = pos2gua_[zs_];

    for (int i = 0; i < ndiff; ++i) {
        guaIndex = cb::remain(9, guaIndex += nk);
    }
    //
    int nPos = gua2pos_[guaIndex];

    // 如果门转到了中宫，也是按照寄宫处理。
    if (nPos == 8) {
        nPos = jg_;
    }

    int menIndex = get_index(bm_turn_, 8, bm_pre_[startPos]);
    for (int i = 0; i < 8; ++i, nPos--, ++menIndex) {
        bm_re_[cb::remain(8, nPos)] = bm_turn_[cb::remain(8, menIndex)];
    }
}
// 排八神
void CQiMenV1::gen_bs()
{
    int nxing = jx_pre_[zf_];
    if (nxing == 4) {
        nxing = jx_pre_[jg_];
    }
    // 看看值符被转到哪里了
    int sIndex = get_index(jx_re_, 9, nxing);
    int nk = 1;
    if (!is_yin_) {
        nk = -1;
    }
    for (int i = 0; i < 9; ++i, sIndex += nk) {
        if (i == 4) {
            ++i;
        }
        bs_re_[cb::remain(8, sIndex)] = i;
    }
}
// 排天盘
void CQiMenV1::gen_tp()
{
    // 当前九星宫位的天盘就是：当前星原来宫位中的地盘
    for (int i = 0; i < 8; ++i) {
        t_[i] = d_[gua2pos_[jx_re_[i]]];
    }
}
// 排旬空马星
void CQiMenV1::gen_other()
{
    get_xk(get_jz(pcal_->get_sz().yg_, pcal_->get_sz().yz_), xk_[0], xk_[1]);
    get_xk(get_jz(pcal_->get_sz().mg_, pcal_->get_sz().mz_), xk_[2], xk_[3]);
    get_xk(get_jz(pcal_->get_sz().dg_, pcal_->get_sz().dz_), xk_[4], xk_[5]);
    get_xk(get_jz(pcal_->get_sz().hg_, pcal_->get_sz().hz_), xk_[6], xk_[7]);

    kw_[0] = zhi_[xk_[6]];
    kw_[1] = zhi_[xk_[7]];

    // 马星
    int nchongzhi = dzc_[sanhe_[pcal_->get_sz().hz_]];
    mx_ = zhi_[nchongzhi];
}
}   // namespace cppbox
