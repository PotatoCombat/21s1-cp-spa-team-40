#include "common/model/condition/Condition.h"

Condition::Condition(ConditionType condType) : condType(condType) {}

ConditionType Condition::getConditionType() { return this->condType; }
