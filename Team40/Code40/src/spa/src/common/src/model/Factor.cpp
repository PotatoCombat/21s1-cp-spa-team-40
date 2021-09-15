#include "common/model/Factor.h"

Factor::Factor(FactorType factorType) : factorType(factorType) {}

// getters
FactorType Factor::getFactorType() { return factorType; }
