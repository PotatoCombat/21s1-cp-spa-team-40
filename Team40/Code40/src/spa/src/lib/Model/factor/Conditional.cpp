#include "Conditional.h"

Conditional::Conditional(ConditionalType condType, bool value) : condType(condType), value(value) {}

ConditionalType Conditional::getConditionalType() { return this->condType; }

bool Conditional::getValue() { return this->value; }
