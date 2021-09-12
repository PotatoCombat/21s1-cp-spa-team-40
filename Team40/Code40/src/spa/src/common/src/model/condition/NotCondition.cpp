#include "common/model/condition/NotCondition.h"

NotCondition::NotCondition(Condition *condition)
    : condition(condition),
      Condition(ConditionType::NOT, !condition->getValue()) {}

Condition *NotCondition::getPrimaryCondition() { return condition; }