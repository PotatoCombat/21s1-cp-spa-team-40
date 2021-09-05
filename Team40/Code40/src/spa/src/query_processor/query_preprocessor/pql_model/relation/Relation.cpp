#include "Relation.h"

Relation::Relation(Reference r1, Reference r2, RelationType type) : ref1(r1), ref2(r2), type(type) {}

Reference Relation::getFirstReference() { return this->ref1; }
Reference Relation::getSecondReference() { return this->ref2; }

RelationType Relation::getRelationType() { return this->type; }
