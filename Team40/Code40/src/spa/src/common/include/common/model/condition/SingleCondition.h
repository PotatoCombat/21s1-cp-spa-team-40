#pragma once
#include "Condition.h"
#include "common/model/relation/Relation.h"

class SingleCondition : public Condition {
private:
    Relation *rel;
    bool value;

public:
    SingleCondition(Relation *rel);
};
