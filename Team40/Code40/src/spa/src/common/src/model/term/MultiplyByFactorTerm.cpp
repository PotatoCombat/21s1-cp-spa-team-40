#include "common/model/term/MultiplyByFactorTerm.h"

// Note that value is initialized to a dummy value here
MultiplyByFactorTerm::MultiplyByFactorTerm(Term *term, Factor *factor)
    : term(term), Term(TermType::MULTIPLY_TERM_BY_FACTOR, factor){};

Term *MultiplyByFactorTerm::getTerm() { return term; }
