#include "Relation.h"

Relation::Relation(Reference r1, Reference r2) : ref1(r1), ref2(r2) {}

Reference Relation::getFirstReference() { return ref1; }
Reference Relation::getSecondReference() { return ref2; }
