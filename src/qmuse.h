#ifndef QM_USE_H
#define QM_USE_H

#include "csp_base.hpp"
#include "print.h"

class CQimenUse
{
public:
    CQimenUse() = default;
    ~CQimenUse() = default;

public:
    bool run(const CmdParam& param);

private:
    CCmdPrint print_;
};

#endif
