#include "query_processor/model/Clause.h"

Clause::Clause(ClauseType type, Reference first, Reference second,
               string pattern, bool exactMatch)
    : type(type), first(first), second(second), pattern(pattern),
      exactMatch(exactMatch) {}

Clause::Clause(ClauseType type, Reference first, Reference second)
    : Clause(type, first, second, Clause::DEFAULT_PATTERN, Clause::DEFAULT_EXACT_MATCH) {}

ClauseType Clause::getType() { return this->type; }

Reference *Clause::getFirstReference() { return &this->first; }

Reference *Clause::getSecondReference() { return &this->second; }

string Clause::getPattern() { return this->pattern; }

bool Clause::isExactMatch() { return this->exactMatch; }

bool Clause::equals(Clause &other) {
    return this->getFirstReference()->equals(*(other.getFirstReference())) &&
           this->getSecondReference()->equals(*(other.getSecondReference())) &&
           this->getType() == other.getType();
}

Clause *Clause::copy() {
    Reference *ref1 = this->first.copy();
    Reference *ref2 = this->second.copy();
    return new Clause(this->type, *ref1, *ref2, this->pattern, this->exactMatch);
}

Clause::~Clause() { }
