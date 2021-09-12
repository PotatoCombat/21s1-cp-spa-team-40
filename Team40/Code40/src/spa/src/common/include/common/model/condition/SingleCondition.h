#pragma once
#include "Condition.h"
#include "common/model/relation/Relation.h"

class SingleCondition : public Condition {
private:
    Relation *relation;

public:
    explicit SingleCondition(Relation *relation);
    Relation *getRelation() override;
};
