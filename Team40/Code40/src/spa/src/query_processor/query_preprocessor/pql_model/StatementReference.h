#pragma once

#include "Reference.h"

using namespace std;

// SYNONYM / UNDERSCORE / INTEGER
class StatementReference : public Reference {
public:
    StatementReference(string value) : Reference(value) {}

    ReferenceType getType() { return ReferenceType::STMT_REF; }
};
