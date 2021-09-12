#include "common/model/condition/SingleCondition.h"

SingleCondition::SingleCondition(Relation *relation)
    : relation(relation),
      Condition(ConditionType::SINGLE, relation->getValue()) {}

Relation *SingleCondition::getRelation() { return relation; }
