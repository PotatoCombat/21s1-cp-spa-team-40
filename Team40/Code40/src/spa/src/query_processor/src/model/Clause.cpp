#include "query_processor/model/Clause.h"

const vector<string> Clause::DEFAULT_PATTERN = vector<string>();
const bool Clause::DEFAULT_EXACT_MATCH = true;

// pattern assign clause
Clause::Clause(Reference first, Reference second, vector<string> pattern,
               bool exactMatch)
    : Clause(ClauseType::PATTERN, first, second, pattern, exactMatch) {}

// pattern if / while clause
Clause::Clause(Reference first, Reference second)
    : Clause(ClauseType::PATTERN, first, second, DEFAULT_PATTERN,
             DEFAULT_EXACT_MATCH) {}

// such that / with clause
Clause::Clause(ClauseType type, Reference first, Reference second)
    : Clause(type, first, second, DEFAULT_PATTERN, DEFAULT_EXACT_MATCH) {}

Clause::Clause(ClauseType type, Reference first, Reference second,
               vector<string> pattern, bool exactMatch)
    : type(type), first(first), second(second), pattern(pattern),
      exactMatch(exactMatch) {}

ClauseType Clause::getType() { return this->type; }

Reference *Clause::getFirstReference() { return &this->first; }

Reference *Clause::getSecondReference() { return &this->second; }

vector<string> Clause::getPattern() { return this->pattern; }

bool Clause::isExactMatch() { return this->exactMatch; }

bool Clause::equals(Clause &other) {
    return this->getType() == other.getType() &&
           this->getFirstReference()->equals(*(other.getFirstReference())) &&
           this->getSecondReference()->equals(*(other.getSecondReference())) &&
           this->getPattern() == other.getPattern() &&
           this->isExactMatch() == other.isExactMatch();
}

Clause *Clause::copy() {
    Reference *r1 = this->first.copy();
    Reference *r2 = this->second.copy();
    return new Clause(this->type, *r1, *r2, this->pattern, this->exactMatch);
}

Clause::~Clause() {}
