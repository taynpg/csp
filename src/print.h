#ifndef PRINT_BASE_H
#define PRINT_BASE_H

#ifndef WHAT_FUCK
#include "ColorConsole.hpp"
#endif

#include "qimen.h"

using namespace csp;

class QimenPrint
{
public:
    QimenPrint();
    ~QimenPrint();

public:
    void run(const std::shared_ptr<Qimen>& qm, int type);

private:
    void print_one(int nGong);
    void print_two(int nGong);
    void print_three(int nGong);
    void print_base();
    void print_other();

private:
    std::shared_ptr<Qimen> qm_{};
    int type_{};
};

#endif
