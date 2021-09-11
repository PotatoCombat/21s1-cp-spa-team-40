#include "common/model/term/DivideByFactorTerm.h"

DivideByFactorTerm::DivideByFactorTerm(Term *term, Factor *factor)
    : term(term), Term(TermType::MODULO_TERM_BY_FACTOR, factor){};

int DivideByFactorTerm::getValue() {
    if (!hasComputedValue) {
        value = term->getValue() / getFactor()->getValue();
        hasComputedValue = true;
    }
    return value;
}

Term *DivideByFactorTerm::getTerm() { return term; }