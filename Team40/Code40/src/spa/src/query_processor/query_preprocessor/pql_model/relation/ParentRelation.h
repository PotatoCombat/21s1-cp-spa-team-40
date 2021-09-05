#pragma once

#include "Relation.h"

class ParentRelation : public Relation {
public:
    ParentRelation(Reference r1, Reference r2) : Relation(r1, r2, RelationType::PARENT) {}
};
