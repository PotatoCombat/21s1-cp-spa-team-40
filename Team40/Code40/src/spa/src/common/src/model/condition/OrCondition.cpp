#include "common/model/condition/OrCondition.h"

OrCondition::OrCondition(Condition *cond1, Condition *cond2)
    : cond1(cond1), cond2(cond2), Condition(ConditionType::OR) {}

Condition *OrCondition::getPrimaryCondition() { return cond1; }
Condition *OrCondition::getSecondaryCondition() { return cond2; }
