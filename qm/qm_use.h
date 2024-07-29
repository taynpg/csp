#ifndef QM_USE_H
#define QM_USE_H

#include "../csp_base.hpp"
#include "print_base.h"

class CQimenUse
{
public:
    CQimenUse() = default;
    ~CQimenUse() = default;

public:
    bool run(const CMDParam& param);

private:
    CCmdPrint print_;
};

#endif
