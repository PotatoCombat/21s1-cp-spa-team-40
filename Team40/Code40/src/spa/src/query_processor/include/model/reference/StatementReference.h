#pragma once

#include "Reference.h"

class StatementReference : public Reference {
public:
    StatementReference(string value, DesignEntityType type)
        : Reference(value, type) {}
};
