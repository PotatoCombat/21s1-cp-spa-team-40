#include "Term.h"

Term::Term(TermType termType) : termType(termType) {}

TermType Term::getTermType() { return this->termType; }
