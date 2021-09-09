#pragma once

#include "Reference.h"
#include "RelationType.h"

using namespace std;

class Relation {
private:
    RelationType type;
    Reference *first;
    Reference *second;

public:
    Relation() = default;
    Relation(RelationType type, Reference *first, Reference *second);
    Reference *getFirstReference();
    Reference *getSecondReference();
    RelationType getType();
    bool equals(Relation &other);
};
