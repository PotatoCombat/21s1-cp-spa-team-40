#include "NotCondition.h"

NotCondition::NotCondition(Condition *cond)
    : cond(cond), Condition(ConditionType::NOT, !cond->getValue()) {}
