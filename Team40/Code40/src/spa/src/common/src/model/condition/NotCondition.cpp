#include "common/model/condition/NotCondition.h"

NotCondition::NotCondition(Condition *condition)
    : condition(condition), Condition(ConditionType::NOT) {}

Condition *NotCondition::getPrimaryCondition() { return condition; }