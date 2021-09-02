#pragma once

#include "Relation.h"

class FollowsRelation : public Relation {
public:
    FollowsRelation(Reference r1, Reference r2) : Reference(r1, r2) {}
    RelationType getRelationType() { return RelationType::FOLLOWS; }
};
