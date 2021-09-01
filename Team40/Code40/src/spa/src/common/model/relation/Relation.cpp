#include "Relation.h"

Relation::Relation(RelationType relType, bool value)
    : relType(relType), value(value) {}

RelationType Relation::getRelationType() { return this->relType; }

bool Relation::getValue() { return this->value; }
