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
    static std::string zs(int index);
    // 获取数字中文(零，一...)
    static std::string num(int index);
    // 获取阴历月份名称(正, 二, ...)
    static std::string mon(int index);
    // 获取阴历月份名称(正月, 二月, ...)
    static std::string mon2(int index);
    // 获取二十四节气名称，下标从公历 1 月的小寒开始
    static std::string jq(int index);
    // 获取农历日名称
    static std::string lunar_day(int index);
    // 获取天干
    static std::string gan(int index);
    // 获取地支
    static std::string zhi(int index);
    // 获取六十甲子
    static std::string jz(int index);
    // 获取九星
    static std::string jx(int index);
    // 获取八门
    static std::string bm(int index);
    // 获取八神
    static std::string bs(int index);
    // 获取空亡
    static std::string kw();
    // 获取马星
    static std::string mx();
    // 获取卦名
    static std::string gua(int index);
};
}   // namespace cppbox
#endif
