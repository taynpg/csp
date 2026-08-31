#pragma once

#include <qimen.h>

class JsonExport {
public:
    JsonExport() = default;
    ~JsonExport() = default;

public:
    bool exportJson(const std::shared_ptr<csp::Qimen>& qm, int type);
};
