#include <iostream>
#include "common/cmd_show.h"

int main()
{
    std::array<int, 6> array{ 1, 0, 1, 0, 1, 0 };

    CGuaPrint print;
    print.show_six(array);

	return 0; 
}