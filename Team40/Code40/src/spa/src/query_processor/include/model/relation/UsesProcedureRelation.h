#pragma once

#include "Relation.h"

class UsesProcedureRelation : public Relation {
public:
    UsesProcedureRelation(EntityReference *ref1, EntityReference *ref2)
        : Relation(ref1, ref2) {}
    RelationType getType() { return RelationType::USES_P; }
};
