#include "common/model/condition/AndCondition.h"

AndCondition::AndCondition(Condition *cond1, Condition *cond2)
    : cond1(cond1), cond2(cond2),
      Condition(ConditionType::AND, cond1->getValue() && cond2->getValue()) {}

Condition *AndCondition::getPrimaryCondition() { return cond1; }
Condition *AndCondition::getSecondaryCondition() { return cond2; }