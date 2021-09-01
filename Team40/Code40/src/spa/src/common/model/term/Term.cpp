#include "Term.h"

Term::Term(TermType termType)
    : termType(termType), hasComputedValue(false), value(0) {}

TermType Term::getTermType() { return this->termType; }
