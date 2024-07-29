#ifndef YX_BASE_HEADER
#define YX_BASE_HEADER

#if defined(DYNAMIC_DLL)
#if defined(_MSC_VER)
#define CPP_YX_BASE_EXPORT __declspec(dllexport)
#define CPP_YX_BASE_IMPORT __declspec(dllimport)
#else
#define CPP_YX_BASE_EXPORT __attribute__((visibility("default")))
#define CPP_YX_BASE_IMPORT __attribute__((visibility("default")))
#endif

#ifdef CPP_YX_BASE_LIB
#define CPP_YX_BASE_API CPP_YX_BASE_EXPORT
#else
#define CPP_YX_BASE_API CPP_YX_BASE_IMPORT
#endif
#else
#define CPP_YX_BASE_API
#if defined(_MSC_VER)
#pragma warning(disable : 4251)
#endif
#endif

namespace cppbox {

// 金水木火土 数字代表 12345
enum WUXING_STATE {
    KE,          // 克别人
    BEI_KE,      // 被克
    SHENG,       // 生别人
    BEI_SHENG,   // 被生
    TONG,        // 相同
    NUM_ERROR    // 数字不正确
};

class CPP_YX_BASE_API CYXBase
{
public:
    CYXBase();
    ~CYXBase();

public:
    static const int* zsj();
    // 金水木火土 数字代表 12345
    WUXING_STATE judge(int me, int other);
};
}   // namespace cppbox

#endif
