#include "model/condition/SingleCondition.h"

SingleCondition::SingleCondition(Relation *rel)
    : rel(rel), Condition(ConditionType::SINGLE, rel->getValue()) {}
