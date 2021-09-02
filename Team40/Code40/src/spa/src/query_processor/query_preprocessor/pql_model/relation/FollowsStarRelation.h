#pragma once

#include "Relation.h"

class FollowsStarRelation : public Relation {
public:
    FollowsStarRelation(Reference r1, Reference r2) : Reference(r1, r2) {}
    RelationType getRelationType() { return RelationType::FOLLOWS_T; }
};
