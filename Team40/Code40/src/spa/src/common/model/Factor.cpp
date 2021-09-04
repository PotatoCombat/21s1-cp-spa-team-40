#include "Factor.h"

// constructor
Factor::Factor(FactorType factorType, int value, bool knownValue)
    : factorType(factorType), value(value), knownValue(knownValue) {}

// getters
const FactorType Factor::getFactorType() { return this->factorType; }

const int Factor::getValue() { return this->value; }
