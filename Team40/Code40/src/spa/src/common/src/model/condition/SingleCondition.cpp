#include "common/model/condition/SingleCondition.h"

SingleCondition::SingleCondition(Relation *relation)
    : relation(relation), Condition(ConditionType::SINGLE) {}

Relation *SingleCondition::getRelation() { return relation; }
