#ifndef CSP_HEADER_H
#define CSP_HEADER_H

#define CSP_VERSION "v1.3.3"

#include "box_calendar/calendar_base.h"

#include <string>

struct CMDParam {
    cppbox::CDateTime datetime{};
    int               nJu{};
    int               nType = -1;
    bool              isAutoDate{false};
    std::string       str_datetime{};
    int               calendar_type{1};
};

#endif
