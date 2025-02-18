#ifndef QM_V3_HEADER
#define QM_V3_HEADER

#include "qm_v1.h"

namespace csp {
// V3 : 转盘 时家 拆补法
class QimenV3 : public QimenV1
{
public:
    QimenV3();
    ~QimenV3() override;

public:
    // 排盘
    bool generate(const DateTime& dt, int ju, CalendarType ct) override;

private:
    bool cal_ju(const DateTime& dt);
};
}   // namespace csp

#endif