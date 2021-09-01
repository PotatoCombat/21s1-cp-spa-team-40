#include "Conditional.h"

Conditional::Conditional(ConditionalType condType) : condType(condType) {}

ConditionalType Conditional::getConditionalType() { return this->condType; }
