#include "common/model/condition/Condition.h"

Condition::Condition(ConditionType condType, bool value)
    : condType(condType), value(value) {}

ConditionType Condition::getConditionType() { return this->condType; }

bool Condition::getValue() { return this->value; }

Relation *Condition::getRelation() {
    throw runtime_error("This method is not available for this ConditionType.");
}
