#include "common/model/term/ModuloByFactorTerm.h"

ModuloByFactorTerm::ModuloByFactorTerm(Term *term, Factor *factor)
    : term(term), Term(TermType::MODULO_TERM_BY_FACTOR, factor){};

Term *ModuloByFactorTerm::getTerm() { return term; }