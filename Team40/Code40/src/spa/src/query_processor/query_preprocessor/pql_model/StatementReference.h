#pragma once

#include "Reference.h"
#include <string>

using namespace std;

// SYNONYM / UNDERSCORE / INTEGER
class StatementReference : public Reference {
public:
    StatementReference(string value) : Reference(value) {}
};
