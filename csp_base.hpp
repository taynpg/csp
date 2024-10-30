#ifndef CSP_HEADER_H
#define CSP_HEADER_H

#define CSP_VERSION "v1.4.1"

#include "box_calendar/calendar_base.h"

#include <string>

using namespace cppbox;

struct CMDParam {
    CDateTime dt_{};
    int ju_{};
    int type_ = -1;
    bool is_auto_date_{false};
    std::string str_dt_{};
    int cal_type_{1};
};

#endif
