#include "model/term/Term.h"

Term::Term(TermType termType)
    : termType(termType), hasComputedValue(false), value(0) {}

Term::Term(TermType termType, int value) : termType(termType), value(value) {
    this->hasComputedValue = true;
}

TermType Term::getTermType() { return this->termType; }
