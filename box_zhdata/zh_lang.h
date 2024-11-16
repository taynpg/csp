#ifndef ZHDATA_HEADER_
#define ZHDATA_HEADER_

#include <string>
namespace cppbox {

class CZhData
{
public:
    CZhData() = default;
    ~CZhData() = default;

public:
    // 获取十二长生(长生、沐浴...)
    static std::wstring zs(int index);
    // 获取数字中文(零，一...)
    static std::wstring num(int index);
    // 获取阴历月份名称(正, 二, ...)
    static std::wstring mon(int index);
    // 获取阴历月份名称(正月, 二月, ...)
    static std::wstring mon2(int index);
    // 获取二十四节气名称，下标从公历 1 月的小寒开始
    static std::wstring jq(int index);
    // 获取农历日名称
    static std::wstring lunar_day(int index);
    // 获取天干
    static std::wstring gan(int index);
    // 获取地支
    static std::wstring zhi(int index);
    // 获取六十甲子
    static std::wstring jz(int index);
    // 获取九星
    static std::wstring jx(int index);
    // 获取八门
    static std::wstring bm(int index);
    // 获取八神
    static std::wstring bs(int index);
    // 获取空亡
    static std::wstring kw();
    // 获取马星
    static std::wstring mx();
    // 获取卦名
    static std::wstring gua(int index);
};
}   // namespace cppbox
#endif