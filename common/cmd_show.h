#ifndef CMD_SHOW_HEADER
#define CMD_SHOW_HEADER
#include <array>

class CGuaPrint
{
public:
    CGuaPrint() = default;
    ~CGuaPrint();

public:
    void show(bool qian);
    void show_six(const std::array<int, 6>& array);
};

#endif