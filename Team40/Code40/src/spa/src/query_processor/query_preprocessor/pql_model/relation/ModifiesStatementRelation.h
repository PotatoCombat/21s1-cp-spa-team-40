#pragma once

#include "Relation.h"

class ModifiesStatementRelation : public Relation {
public:
    ModifiesStatementRelation(Reference r1, Reference r2) : Relation(r1, r2, RelationType::MODIFIES_S) {}
};
