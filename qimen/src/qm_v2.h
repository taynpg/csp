#ifndef QM_V2_HEADER
#define QM_V2_HEADER

#include "qm_v1.h"

namespace csp {
// V2 : 转盘 时家 阴盘
class QimenV2 : public QimenV1
{
public:
    QimenV2();
    ~QimenV2() override;

public:
    // 排盘
    bool generate(const DateTime& dt, int ju) override;

private:
    bool cal_ju(const DateTime& dt);
    void cal_yujiang();
};
}   // namespace csp

#endif
