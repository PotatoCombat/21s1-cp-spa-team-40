#pragma once

#include "StatementReference.h"

class WildcardStatement : public StatementReference {
public:
    WildcardStatement(DesignEntityType type)
        : StatementReference("_", type) {}
    bool isSynonym() { return false; }
};
