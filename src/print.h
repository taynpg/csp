#ifndef PRINT_BASE_H
#define PRINT_BASE_H

#include "qimen.h"
#include "ColorConsole.hpp"

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
