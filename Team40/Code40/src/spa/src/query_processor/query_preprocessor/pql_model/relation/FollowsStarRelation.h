#pragma once

#include "Relation.h"

class FollowsStarRelation : public Relation {
public:
    FollowsStarRelation(Reference r1, Reference r2) : Relation(r1, r2, RelationType::FOLLOWS_T) {}
};
