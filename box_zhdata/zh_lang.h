#ifndef ZHDATA_HEADER_
#define ZHDATA_HEADER_

#include <string>

#if defined (DYNAMIC_DLL)
#if defined(_MSC_VER)
#  define CPP_ZHDATA_EXPORT __declspec(dllexport)
#  define CPP_ZHDATA_IMPORT __declspec(dllimport)
#else
#  define CPP_ZHDATA_EXPORT     __attribute__((visibility("default")))
#  define CPP_ZHDATA_IMPORT     __attribute__((visibility("default")))
#endif

#ifdef CPP_ZHDATA_LIB
#define CPP_ZHDATA_API CPP_ZHDATA_EXPORT
#else
#define CPP_ZHDATA_API CPP_ZHDATA_IMPORT
#endif
#else
    #define CPP_ZHDATA_API
    #if defined(_MSC_VER)
        #pragma warning(disable: 4251)
    #endif
#endif

namespace cppbox {

	class CPP_ZHDATA_API CZhData {
	public:
		CZhData() = default;
		~CZhData() = default;
	public:
		// 获取十二长生(长生、沐浴...)
		static std::string ZhZhangSheng(int nIndex);
		// 获取数字中文(零，一...)
		static std::string ZhNumber(int nIndex);
		// 获取阴历月份名称(正, 二, ...)
		static std::string ZhMon(int nIndex);
		// 获取阴历月份名称(正月, 二月, ...)
		static std::string ZhMonS(int nIndex);
		// 获取二十四节气名称，下标从公历 1 月的小寒开始
		static std::string ZhJieQi(int nIndex);
		// 获取农历日名称
		static std::string ZhLunarDay(int nIndex);
		// 获取天干
		static std::string ZhGan(int nIndex);
		// 获取地支
		static std::string ZhZhi(int nIndex);
		// 获取六十甲子
		static std::string ZhJiaZi(int nIndex);
		// 获取九星
		static std::string ZhJiuXing(int nIndex);
		// 获取八门
		static std::string ZhBaMen(int nIndex);
		// 获取八神
		static std::string ZhBaShen(int nIndex);
		// 获取空亡
		static std::string ZhKongWang();
		// 获取马星
		static std::string ZhMaXing();
		// 获取卦名
		static std::string ZhGua(int nIndex);
	};
}
#endif