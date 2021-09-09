#include "common/model/term/MultiplyByFactorTerm.h"

// Note that value is initialized to a dummy value here
MultiplyByFactorTerm::MultiplyByFactorTerm(Term *term, Factor *factor)
    : term(term), Term(TermType::MULTIPLY_TERM_BY_FACTOR, factor){};

int MultiplyByFactorTerm::getValue() {
    if (!hasComputedValue) {
        value = term->getValue() * getFactor()->getValue();
        hasComputedValue = true;
    }
    return value;
}
