#include "zh_lang.h"

const wchar_t* gStr_num[] = {L"零", L"一", L"二", L"三", L"四", L"五", L"六", L"七", L"八", L"九", L"十"};
const wchar_t* gStr_ymc[] = {L"正", L"二", L"三", L"四", L"五", L"六", L"七", L"八", L"九", L"十", L"冬", L"腊"};
const wchar_t* gStr_jq[] = {L"小寒", L"大寒", L"立春", L"雨水", L"惊蛰", L"春分", L"清明", L"谷雨",
                            L"立夏", L"小满", L"芒种", L"夏至", L"小暑", L"大暑", L"立秋", L"处暑",
                            L"白露", L"秋分", L"寒露", L"霜降", L"立冬", L"小雪", L"大雪", L"冬至"};
const wchar_t* gStr_lua[] = {L"初一", L"初二", L"初三", L"初四", L"初五", L"初六", L"初七", L"初八", L"初九", L"初十",
                             L"十一", L"十二", L"十三", L"十四", L"十五", L"十六", L"十七", L"十八", L"十九", L"二十",
                             L"廿一", L"廿二", L"廿三", L"廿四", L"廿五", L"廿六", L"廿七", L"廿八", L"廿九", L"三十"};
const wchar_t* gStr_gan[] = {L"甲", L"乙", L"丙", L"丁", L"戊", L"己", L"庚", L"辛", L"壬", L"癸"};
const wchar_t* gStr_zhi[] = {L"子", L"丑", L"寅", L"卯", L"辰", L"巳", L"午", L"未", L"申", L"酉", L"戌", L"亥"};
const wchar_t* gStr_Jiazi[] = {L"甲子", L"乙丑", L"丙寅", L"丁卯", L"戊辰", L"己巳", L"庚午", L"辛未", L"壬申", L"癸酉",
                               L"甲戌", L"乙亥", L"丙子", L"丁丑", L"戊寅", L"己卯", L"庚辰", L"辛巳", L"壬午", L"癸未",
                               L"甲申", L"乙酉", L"丙戌", L"丁亥", L"戊子", L"己丑", L"庚寅", L"辛卯", L"壬辰", L"癸巳",
                               L"甲午", L"乙未", L"丙申", L"丁酉", L"戊戌", L"己亥", L"庚子", L"辛丑", L"壬寅", L"癸卯",
                               L"甲辰", L"乙巳", L"丙午", L"丁未", L"戊申", L"己酉", L"庚戌", L"辛亥", L"壬子", L"癸丑",
                               L"甲寅", L"乙卯", L"丙辰", L"丁巳", L"戊午", L"己未", L"庚申", L"辛酉", L"壬戌", L"癸亥"};
const wchar_t* gStr_Jiuxing[] = {L"天蓬", L"天芮", L"天冲", L"天辅", L"天禽", L"天心", L"天柱", L"天任", L"天英"};
const wchar_t* gStr_Bamen[] = {L"休门", L"死门", L"伤门", L"杜门", L"死门", L"开门", L"惊门", L"生门", L"景门"};
const wchar_t* gStr_Bashen[] = {L"值符", L"腾蛇", L"太阴", L"六合", L"腾蛇", L"白虎", L"玄武", L"九地", L"九天"};
const wchar_t* gStr_ZhangSheng[] = {L"长生", L"沐浴", L"冠带", L"临官", L"帝旺", L"衰", L"病", L"死", L"墓", L"绝", L"胎", L"养"};
const wchar_t* gStr_Gua[] = {L"坎", L"坤", L"震", L"巽", L"中", L"乾", L"兑", L"艮", L"离"};

namespace cppbox {

// 获取卦名
std::wstring CZhData::gua(int index)
{
    if (index < 0 || index > 8) {
        return L"";
    }
    std::wstring result(gStr_Gua[index]);
    return result;
}

std::wstring CZhData::zs(int index)
{
    if (index < 0 || index > 11) {
        return L"";
    }
    std::wstring result(gStr_ZhangSheng[index]);
    return result;
}

std::wstring CZhData::num(int index)
{
    if (index < 0 || index > 10) {
        return L"";
    }
    std::wstring result(gStr_num[index]);
    return result;
}
std::wstring CZhData::mon(int index)
{
    if (index < 0 || index > 11) {
        return L"";
    }
    std::wstring result(gStr_ymc[index]);
    return result;
}
std::wstring CZhData::mon2(int index)
{
    if (index < 0 || index > 11) {
        return L"";
    }
    std::wstring result(gStr_ymc[index]);
    result.append(L"月");
    return result;
}
std::wstring CZhData::jq(int index)
{   // 获取二十四节气名称，下标从公历 1 月的小寒开始
    if (index < 0 || index > 23) {
        return L"";
    }
    std::wstring result(gStr_jq[index]);
    return result;
}

std::wstring CZhData::lunar_day(int index)
{   // 获取农历日名称
    if (index < 0 || index > 29) {
        return L"";
    }
    std::wstring result(gStr_lua[index]);
    return result;
}
// 获取天干
std::wstring CZhData::gan(int index)
{
    if (index < 0 || index > 9) {
        return L"";
    }
    std::wstring result(gStr_gan[index]);
    return result;
}
// 获取地支
std::wstring CZhData::zhi(int index)
{
    if (index < 0 || index > 11) {
        return L"";
    }
    std::wstring result(gStr_zhi[index]);
    return result;
}
// 获取六十甲子
std::wstring CZhData::jz(int index)
{
    if (index < 0 || index > 59) {
        return L"";
    }
    std::wstring result(gStr_Jiazi[index]);
    return result;
}

// 获取九星
std::wstring CZhData::jx(int index)
{
    if (index < 0 || index > 8) {
        return L"";
    }
    std::wstring result(gStr_Jiuxing[index]);
    return result;
}
// 获取八门
std::wstring CZhData::bm(int index)
{
    if (index < 0 || index > 8) {
        return L"";
    }
    std::wstring result(gStr_Bamen[index]);
    return result;
}
// 获取八神
std::wstring CZhData::bs(int index)
{
    if (index < 0 || index > 8) {
        return L"";
    }
    std::wstring result(gStr_Bashen[index]);
    return result;
}
// 获取空亡
std::wstring CZhData::kw()
{
    std::wstring result(L"空");
    return result;
}
// 获取马星
std::wstring CZhData::mx()
{
    std::wstring result(L"马");
    return result;
}
}   // namespace cppbox
