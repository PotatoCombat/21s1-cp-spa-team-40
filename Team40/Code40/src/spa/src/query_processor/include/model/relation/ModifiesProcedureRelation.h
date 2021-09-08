#pragma once

#include "Relation.h"

class ModifiesProcedureRelation : public Relation {
public:
    ModifiesProcedureRelation(EntityReference *ref1, EntityReference *ref2)
        : Relation(ref1, ref2) {}
    RelationType getType() { return RelationType::MODIFIES_P; }
};
