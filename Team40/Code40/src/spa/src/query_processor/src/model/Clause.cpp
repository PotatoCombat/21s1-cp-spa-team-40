#include "query_processor/model/Clause.h"

Clause::Clause(ClauseType type, Reference first, Reference second)
    : type(type), first(first), second(second) {}

ClauseType Clause::getType() { return this->type; }

Reference *Clause::getFirstReference() { return &this->first; }

Reference *Clause::getSecondReference() { return &this->second; }

bool Clause::equals(Clause &other) {
    return this->getFirstReference()->equals(*(other.getFirstReference())) &&
           this->getSecondReference()->equals(*(other.getSecondReference())) &&
           this->getType() == other.getType();
}

Clause *Clause::copy() {
    return new Clause(this->type, this->first, this->second);
}

Clause::~Clause() { }
