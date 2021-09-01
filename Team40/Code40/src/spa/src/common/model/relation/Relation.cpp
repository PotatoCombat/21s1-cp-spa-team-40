#include "Relation.h"

Relation::Relation(RelationalType relType, bool value)
    : relType(relType), value(value) {}

RelationType Relational::getRelationType() { return this->relType; }

bool Relation::getValue() { return this->value; }
