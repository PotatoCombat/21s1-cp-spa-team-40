#pragma once

#include "Relation.h"

class FollowsRelation : public Relation {
public:
    FollowsRelation(StatementReference *ref1, StatementReference *ref2)
        : Relation(ref1, ref2) {}
    RelationType getType() { return RelationType::FOLLOWS; }
};
