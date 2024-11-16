#ifndef YX_BASE_HEADER
#define YX_BASE_HEADER

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

class CYXBase
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
