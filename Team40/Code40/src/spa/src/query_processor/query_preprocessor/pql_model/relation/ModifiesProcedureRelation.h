#pragma once

#include "Relation.h"

class ModifiesProcedureRelation : public Relation {
public:
    ModifiesProcedureRelation(Reference r1, Reference r2) : Reference(r1, r2) {}
    RelationType getRelationType() { return RelationType::MODIFIES_P; }
};
