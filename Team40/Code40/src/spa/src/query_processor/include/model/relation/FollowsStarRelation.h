#pragma once

#include "Relation.h"

class FollowsStarRelation : public Relation {
public:
    FollowsStarRelation(StatementReference *ref1, StatementReference *ref2)
        : Relation(ref1, ref2) {}
    RelationType getType() { return RelationType::FOLLOWS_T; }
};
