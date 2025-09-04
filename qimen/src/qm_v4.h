#ifndef QM_V4_HEADER
#define QM_V4_HEADER

#include "qm_v1.h"

namespace csp {
// V4 : 转盘 时家 茅山法
class QimenV4 : public QimenV1
{
public:
    QimenV4();
    ~QimenV4() override;

public:
    // 排盘
    bool generate(const DateTime& dt, int ju) override;

private:
    bool cal_ju(const DateTime& dt);
    int cal_yuan(tyme::SolarTime* dt);
};
}   // namespace csp

#endif