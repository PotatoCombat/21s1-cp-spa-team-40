#pragma once

#include "../Reference.h"

enum class RelationType {
    UNKNOWN,
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
protected:
    Reference ref1;
    Reference ref2;

public:
    Relation(Reference r1, Reference r2);

    Reference getFirstReference();
    Reference getSecondReference();
    virtual RelationType getRelationType();
};
