#include "common/model/condition/Condition.h"

Condition::Condition(ConditionType condType, bool value)
    : condType(condType), value(value) {}

ConditionType Condition::getConditionType() { return this->condType; }

bool Condition::getValue() { return this->value; }
