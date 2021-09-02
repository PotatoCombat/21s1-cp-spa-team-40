#pragma once

#include "Relation.h"

class UsesProcedureRelation : public Relation {
public:
    UsesProcedureRelation(Reference r1, Reference r2) : Reference(r1, r2) {}
    RelationType getRelationType() { return RelationType::USES_P; }
};
