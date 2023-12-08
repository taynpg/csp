#ifndef PRINT_BASE_H
#define PRINT_BASE_H

#include "qm_base.h"

using namespace cppbox;

class CCmdPrint {
public:
    CCmdPrint();
    ~CCmdPrint();

public:
    void Run(cppbox::CQimen* qm, int nType);

private:
    void PrintOne(int nGong);
    void PrintTwo(int nGong);
    void PrintThree(int nGong);
    void PrintBase();
    void PrintOther();

private:
    cppbox::CQimen* m_qm{};
    int             m_type{};
};

#endif
