#include "Relation.h"

Relation::Relation(RelationType type, Reference *first, Reference *second)
    : type(type), first(first), second(second) {}

RelationType Relation::getType() { return this->type; }

Reference *Relation::getFirstReference() { return this->first; }

Reference *Relation::getSecondReference() { return this->second; }
