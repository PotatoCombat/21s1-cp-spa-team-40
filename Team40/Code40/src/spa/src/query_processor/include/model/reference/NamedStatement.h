#pragma once

#include "StatementReference.h"

class NamedStatement : public StatementReference {
public:
    NamedStatement(string value, DesignEntityType type)
        : StatementReference(value, type) {}
    bool isSynonym() { return false; }
};
