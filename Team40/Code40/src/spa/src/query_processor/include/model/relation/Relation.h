#pragma once

#include "Reference.h"

using namespace std;

enum class RelationType {
    FOLLOWS,
    FOLLOWS_T,
    PARENT,
    PARENT_T,
    MODIFIES_S,
    MODIFIES_P,
    USES_S,
    USES_P
};

class Relation {
private:
    Reference *first;
    Reference *second;

public:
    Relation(Reference *first, Reference *second);
    Reference *getFirstReference();
    Reference *getSecondReference();
    virtual RelationType getType() = 0;
};
