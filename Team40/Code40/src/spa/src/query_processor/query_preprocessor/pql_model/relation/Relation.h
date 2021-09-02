#pragma once

#include "../Reference.h"

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
    Reference ref1;
    Reference ref2;

public:
    Relation(Reference r1, Reference r2) : ref1(r1), ref2(r2) {}
    Reference getFirstReference() { return ref1; }
    Reference getSecondReference() { return ref2; }
    virtual RelationType getRelationType() = 0;
};
