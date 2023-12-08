#ifndef YX_BASE_HEADER_
#define YX_BASE_HEADER_

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

namespace cppbox {

class CPP_YX_BASE_API CYXBase {
public:
    CYXBase();
    ~CYXBase();

public:
    static const int* ZhangShengJue();
};
}  // namespace cppbox
#endif
