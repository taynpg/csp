#ifndef CSP_HEADER_H
#define CSP_HEADER_H

constexpr auto CSP_VERSION = "v1.4.5";

#include <string>

struct CmdParam {
    int year{};
    int mon{};
    int day{};
    int hour{};
    int min{};
    int sec{};
    int ju{};
    int type = -1;
    int zone = 0;
    bool is_auto{false};
    std::string str_dt{};
    int cal_type{1};
};

#endif
