#pragma once

#include "Relation.h"

class ParentRelation : public Relation {
public:
    ParentRelation(StatementReference* ref1, StatementReference * ref2)
        : Relation(ref1, ref2) {}
    RelationType getType() { return RelationType::PARENT; }
};
