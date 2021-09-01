#include "Relational.h"

Relational::Relational(RelationalType relType, bool value) 
    : relType(relType), value(value) {}

RelationalType Relational::getRelationalType() { return this->relType; }

bool Relational::getValue() { return this->value; }

