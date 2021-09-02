#pragma once

#include "Relation.h"

class ParentStarRelation : public Relation {
public:
    ParentStarRelation(Reference r1, Reference r2) : Reference(r1, r2) {}
    RelationType getRelationType() { return RelationType::PARENT_T; }
};
