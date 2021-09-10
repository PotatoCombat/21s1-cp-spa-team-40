#pragma once

#include "Reference.h"

using namespace std;

enum class ClauseType {
    FOLLOWS,
    FOLLOWS_T,
    PARENT,
    PARENT_T,
    MODIFIES_S,
    MODIFIES_P,
    USES_S,
    USES_P
};

class Clause {
private:
    ClauseType type;
    Reference first;
    Reference second;

public:
    Clause(ClauseType type, Reference first, Reference second);
    Reference *getFirstReference();
    Reference *getSecondReference();
    ClauseType getType();
    bool equals(Clause &other);
};
