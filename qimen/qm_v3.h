//
// Created by taynpg on 2023/12/16.
//

#ifndef CSP_QM_V3_H
#define CSP_QM_V3_H

#include "qm_v1.h"

namespace cppbox {

// V3 : 转盘 时家 拆补法
class CQimenV3 : public CQimenV1
{
public:
    CQimenV3() = default;
    ~CQimenV3() override = default;

public:
    // 传入日期数据
    bool run(const QiParam& info, CalendarType type) override;
};
}   // namespace cppbox
#endif   // CSP_QM_V3_H
