#include "common/model/term/ModuloByFactorTerm.h"

ModuloByFactorTerm::ModuloByFactorTerm(Term *term, Factor *factor)
    : term(term), Term(TermType::MODULO_TERM_BY_FACTOR, factor){};

int ModuloByFactorTerm::getValue() {
    if (!hasComputedValue) {
        value = term->getValue() % getFactor()->getValue();
        hasComputedValue = true;
    }
    return value;
}

Term *ModuloByFactorTerm::getTerm() { return term; }