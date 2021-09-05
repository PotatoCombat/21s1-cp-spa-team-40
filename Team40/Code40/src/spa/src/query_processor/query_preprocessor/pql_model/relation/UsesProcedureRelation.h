#pragma once

#include "Relation.h"

class UsesProcedureRelation : public Relation {
public:
    UsesProcedureRelation(Reference r1, Reference r2) : Relation(r1, r2, RelationType::USES_P) {}
};
