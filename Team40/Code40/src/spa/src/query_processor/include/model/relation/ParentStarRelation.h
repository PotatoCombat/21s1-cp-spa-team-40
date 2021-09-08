#pragma once

#include "Relation.h"

class ParentStarRelation : public Relation {
public:
    ParentStarRelation(StatementReference *ref1, StatementReference *ref2)
        : Relation(ref1, ref2) {}
    RelationType getType() { return RelationType::PARENT_T; }
};
