#include "common/model/Factor.h"

Factor::Factor(FactorType factorType) : factorType(factorType) {
    this->hasComputedValue = false;
}

// getters
FactorType Factor::getFactorType() { return factorType; }

int Factor::getValue() const {
    if (!hasComputedValue) {
        throw runtime_error("This factor does not have a value.");
    }
    return value;
}
