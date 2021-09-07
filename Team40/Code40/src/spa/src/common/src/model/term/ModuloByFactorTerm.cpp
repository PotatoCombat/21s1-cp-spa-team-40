#include "common/model/term/ModuloByFactorTerm.h"

ModuloByFactorTerm::ModuloByFactorTerm(Term *term, Factor factor)
    : factor(factor), term(term), Term(TermType::MODULO_TERM_BY_FACTOR){};

int ModuloByFactorTerm::getValue() {
    if (!hasComputedValue) {
        value = term->getValue() % factor.getValue();
        hasComputedValue = true;
    }
    return value;
}
