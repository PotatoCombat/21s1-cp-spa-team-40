#include "Factor.h"

//constructor
Factor::Factor(FactorType factorType, int value) :factorType(factorType), value(value) {}

//getters
const FactorType Factor::getFactorType() {
	return this->factorType;
}

const int Factor::getValue() {
    return this->value;
}