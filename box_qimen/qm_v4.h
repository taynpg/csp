#ifndef CSP_QM_V4_H
#define CSP_QM_V4_H

#include "qm_v1.h"

namespace cppbox {

// V4 : 转盘 时家 茅山法
class CQimenV4 : public CQiMenV1
{
public:
    CQimenV4() = default;
    ~CQimenV4() override = default;

public:
    // 传入日期数据
    bool run(const QiParam& info, CalendarType type) override;

private:
    // 三元 0 手动, 1下，2中，3上
    int get_yuan(const CDateTime& jie, const CDateTime& datetime);
};
}   // namespace cppbox

#endif