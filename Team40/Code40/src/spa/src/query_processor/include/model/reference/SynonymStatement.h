#pragma once

#include "StatementReference.h"

class SynonymStatement : public StatementReference {
public:
    SynonymStatement(string value, DesignEntityType type)
        : StatementReference(value, type) {}
    bool isSynonym() { return true; }
};
