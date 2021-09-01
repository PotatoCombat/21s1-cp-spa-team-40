#include "Term.h"

Term::Term(TermType termType) : termType(termType), hasComputedValue(false) {}

TermType Term::getTermType() { return this->termType; }
