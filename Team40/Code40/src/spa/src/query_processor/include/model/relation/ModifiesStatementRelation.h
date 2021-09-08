#pragma once

#include "Relation.h"

class ModifiesStatementRelation : public Relation {
public:
    ModifiesStatementRelation(StatementReference *ref1, EntityReference *ref2)
        : Relation(ref1, ref2) {}
    RelationType getType() { return RelationType::MODIFIES_S; }
};
