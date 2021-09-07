#include "model/term/MultiplyByFactorTerm.h"

// Note that value is initialized to a dummy value here
MultiplyByFactorTerm::MultiplyByFactorTerm(Term *term, Factor factor)
    : factor(factor), term(term), Term(TermType::MULTIPLY_TERM_BY_FACTOR){};

int MultiplyByFactorTerm::getValue() {
    if (!hasComputedValue) {
        value = term->getValue() * factor.getValue();
        hasComputedValue = true;
    }
    return value;
}
