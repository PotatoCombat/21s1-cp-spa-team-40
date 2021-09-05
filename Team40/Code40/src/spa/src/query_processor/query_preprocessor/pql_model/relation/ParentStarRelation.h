#pragma once

#include "Relation.h"

class ParentStarRelation : public Relation {
public:
    ParentStarRelation(Reference r1, Reference r2) : Relation(r1, r2, RelationType::PARENT_T) {}
};
