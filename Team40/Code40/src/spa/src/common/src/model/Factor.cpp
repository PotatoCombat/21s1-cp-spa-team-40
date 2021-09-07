#include "common/model/Factor.h"

// constructor
Factor::Factor(FactorType factorType, int value)
    : factorType(factorType), value(value) {
    this->hasComputedValue = true;
}

Factor::Factor(FactorType factorType) : factorType(factorType) {
    this->hasComputedValue = false;
}

// getters
const FactorType Factor::getFactorType() { return this->factorType; }

const int Factor::getValue() { return this->value; }
