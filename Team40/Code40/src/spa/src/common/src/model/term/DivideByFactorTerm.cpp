#include "common/model/term/DivideByFactorTerm.h"

DivideByFactorTerm::DivideByFactorTerm(Term *term, Factor *factor)
    : term(term), Term(TermType::MODULO_TERM_BY_FACTOR, factor){};

Term *DivideByFactorTerm::getTerm() { return term; }