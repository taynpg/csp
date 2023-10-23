#ifndef BOXBASE_HEADER_
#define BOXBASE_HEADER_

#if defined(_MSC_VER)
#  define CPP_BOXBASE_EXPORT __declspec(dllexport)
#  define CPP_BOXBASE_IMPORT __declspec(dllimport)
#else
#  define CPP_BOXBASE_EXPORT     __attribute__((visibility("default")))
#  define CPP_BOXBASE_IMPORT     __attribute__((visibility("default")))
#endif

#ifdef CPP_BOXBASE_LIB
#define CPP_BOXBASE_API CPP_BOXBASE_EXPORT
#else
#define CPP_BOXBASE_API CPP_BOXBASE_IMPORT
#endif

#include <string>
 
namespace cppbox {

class CPP_BOXBASE_API CBase {
public:
    CBase();
    ~CBase();
public:
    // 字符串转码成 UTF-8 编码
    static std::string u8(const std::string& data);
    // 求余数(结果大于0)
    static int getRemainder(int nBase, int nValue);
    // 查找一个数在数组中的位置
    static int getIndex(const int* pData, int nSize, int nValue);
};
}

#endif