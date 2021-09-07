#include "common/model/term/DivideByFactorTerm.h"

DivideByFactorTerm::DivideByFactorTerm(Term *term, Factor factor)
    : factor(factor), term(term), Term(TermType::MODULO_TERM_BY_FACTOR){};

int DivideByFactorTerm::getValue() {
    if (!hasComputedValue) {
        value = term->getValue() / factor.getValue();
        hasComputedValue = true;
    }
    return value;
}
