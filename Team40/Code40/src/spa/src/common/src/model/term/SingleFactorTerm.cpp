#include "common/model/term/SingleFactorTerm.h"

SingleFactorTerm::SingleFactorTerm(Factor *factor)
    : Term(TermType::SINGLE_FACTOR, factor){};

int SingleFactorTerm::getValue() {
    if (!hasComputedValue) {
        value = getFactor()->getValue();
        hasComputedValue = true;
    }
    return value;
}