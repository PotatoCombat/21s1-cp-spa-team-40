#pragma once

#include "Relation.h"

class ModifiesProcedureRelation : public Relation {
public:
    ModifiesProcedureRelation(Reference r1, Reference r2) : Relation(r1, r2, RelationType::MODIFIES_P) {}
};
