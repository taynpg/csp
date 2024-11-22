#include "zh_lang.h"

const char* gStr_num[] = {"零", "一", "二", "三", "四", "五", "六", "七", "八", "九", "十"};
const char* gStr_ymc[] = {"正", "二", "三", "四", "五", "六", "七", "八", "九", "十", "冬", "腊"};
const char* gStr_jq[] = {"小寒", "大寒", "立春", "雨水", "惊蛰", "春分", "清明", "谷雨", "立夏", "小满", "芒种", "夏至",
                         "小暑", "大暑", "立秋", "处暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪", "冬至"};
const char* gStr_lua[] = {"初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十",
                          "十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八", "十九", "二十",
                          "廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十"};
const char* gStr_gan[] = {"甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"};
const char* gStr_zhi[] = {"子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"};
const char* gStr_Jiazi[] = {"甲子", "乙丑", "丙寅", "丁卯", "戊辰", "己巳", "庚午", "辛未", "壬申", "癸酉", "甲戌", "乙亥",
                            "丙子", "丁丑", "戊寅", "己卯", "庚辰", "辛巳", "壬午", "癸未", "甲申", "乙酉", "丙戌", "丁亥",
                            "戊子", "己丑", "庚寅", "辛卯", "壬辰", "癸巳", "甲午", "乙未", "丙申", "丁酉", "戊戌", "己亥",
                            "庚子", "辛丑", "壬寅", "癸卯", "甲辰", "乙巳", "丙午", "丁未", "戊申", "己酉", "庚戌", "辛亥",
                            "壬子", "癸丑", "甲寅", "乙卯", "丙辰", "丁巳", "戊午", "己未", "庚申", "辛酉", "壬戌", "癸亥"};
const char* gStr_Jiuxing[] = {"天蓬", "天芮", "天冲", "天辅", "天禽", "天心", "天柱", "天任", "天英"};
const char* gStr_Bamen[] = {"休门", "死门", "伤门", "杜门", "死门", "开门", "惊门", "生门", "景门"};
const char* gStr_Bashen[] = {"值符", "腾蛇", "太阴", "六合", "腾蛇", "白虎", "玄武", "九地", "九天"};
const char* gStr_ZhangSheng[] = {"长生", "沐浴", "冠带", "临官", "帝旺", "衰", "病", "死", "墓", "绝", "胎", "养"};
const char* gStr_Gua[] = {"坎", "坤", "震", "巽", "中", "乾", "兑", "艮", "离"};

namespace cppbox {

// 获取卦名
std::string CZhData::gua(int index)
{
    if (index < 0 || index > 8) {
        return "";
    }
    std::string result(gStr_Gua[index]);
    return result;
}

std::string CZhData::zs(int index)
{
    if (index < 0 || index > 11) {
        return "";
    }
    std::string result(gStr_ZhangSheng[index]);
    return result;
}

std::string CZhData::num(int index)
{
    if (index < 0 || index > 10) {
        return "";
    }
    std::string result(gStr_num[index]);
    return result;
}
std::string CZhData::mon(int index)
{
    if (index < 0 || index > 11) {
        return "";
    }
    std::string result(gStr_ymc[index]);
    return result;
}
std::string CZhData::mon2(int index)
{
    if (index < 0 || index > 11) {
        return "";
    }
    std::string result(gStr_ymc[index]);
    result.append("月");
    return result;
}
std::string CZhData::jq(int index)
{   // 获取二十四节气名称，下标从公历 1 月的小寒开始
    if (index < 0 || index > 23) {
        return "";
    }
    std::string result(gStr_jq[index]);
    return result;
}

std::string CZhData::lunar_day(int index)
{   // 获取农历日名称
    if (index < 0 || index > 29) {
        return "";
    }
    std::string result(gStr_lua[index]);
    return result;
}
// 获取天干
std::string CZhData::gan(int index)
{
    if (index < 0 || index > 9) {
        return "";
    }
    std::string result(gStr_gan[index]);
    return result;
}
// 获取地支
std::string CZhData::zhi(int index)
{
    if (index < 0 || index > 11) {
        return "";
    }
    std::string result(gStr_zhi[index]);
    return result;
}
// 获取六十甲子
std::string CZhData::jz(int index)
{
    if (index < 0 || index > 59) {
        return "";
    }
    std::string result(gStr_Jiazi[index]);
    return result;
}

// 获取九星
std::string CZhData::jx(int index)
{
    if (index < 0 || index > 8) {
        return "";
    }
    std::string result(gStr_Jiuxing[index]);
    return result;
}
// 获取八门
std::string CZhData::bm(int index)
{
    if (index < 0 || index > 8) {
        return "";
    }
    std::string result(gStr_Bamen[index]);
    return result;
}
// 获取八神
std::string CZhData::bs(int index)
{
    if (index < 0 || index > 8) {
        return "";
    }
    std::string result(gStr_Bashen[index]);
    return result;
}
// 获取空亡
std::string CZhData::kw()
{
    std::string result("空");
    return result;
}
// 获取马星
std::string CZhData::mx()
{
    std::string result("马");
    return result;
}
}   // namespace cppbox
