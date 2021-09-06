#pragma once

#include "Relation.h"

class UsesStatementRelation : public Relation {
public:
    UsesStatementRelation(Reference r1, Reference r2) : Relation(r1, r2, RelationType::USES_S) {}
};
