#include "common/model/term/Term.h"

Term::Term(TermType termType, Factor *factor)
    : termType(termType), factor(factor), hasComputedValue(false), value(0) {}

TermType Term::getTermType() { return termType; }

Factor *Term::getFactor() { return factor; }
