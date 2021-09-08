#include "Relation.h"

Relation::Relation(Reference *first, Reference *second)
    : first(first), second(second) {}

Reference *Relation::getFirstReference() { return this->first; }

Reference *Relation::getSecondReference() { return this->second; }
