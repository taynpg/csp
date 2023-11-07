#ifndef QM_USE_H
#define QM_USE_H

#include "print_base.h"
#include "../csp_base.hpp"

class CQimenUse
{
public:
    CQimenUse() = default;
    ~CQimenUse() = default;
public:
    bool Run(const CMDParam& param);

private:
    CCmdPrint   m_print;
};

#endif