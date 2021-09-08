#pragma once

#include "Relation.h"

class UsesStatementRelation : public Relation {
public:
    UsesStatementRelation(StatementReference *ref1, EntityReference *ref2)
        : Relation(ref1, ref2) {}
    RelationType getType() { return RelationType::USES_S; }
};
