#include "SingleFactorTerm.h"

SingleFactorTerm::SingleFactorTerm(Factor factor)
    : factor(factor), Term(TermType::SINGLE_FACTOR){};

int SingleFactorTerm::getValue() {
    if (!hasComputedValue) {
        value = factor.getValue();
        hasComputedValue = true;
    }
    return value;
}
