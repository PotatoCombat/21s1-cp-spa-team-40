#include "common/model/condition/OrCondition.h"

OrCondition::OrCondition(Condition *cond1, Condition *cond2)
    : cond1(cond1), cond2(cond2),
      Condition(ConditionType::OR, cond1->getValue() || cond2->getValue()) {}
