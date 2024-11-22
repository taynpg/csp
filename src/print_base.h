#ifndef PRINT_BASE_H
#define PRINT_BASE_H

#include "qm_base.h"

using namespace cppbox;

class CCmdPrint
{
public:
    CCmdPrint();
    ~CCmdPrint();

public:
    void run(cppbox::CQimen* qm, int nType);

private:
    void print_one(int nGong);
    void print_two(int nGong);
    void print_three(int nGong);
    void print_base();
    void print_other();

private:
    cppbox::CQimen* qm_{};
    int type_{};
};

#endif
