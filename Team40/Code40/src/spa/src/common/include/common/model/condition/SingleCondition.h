#pragma once
#include "Condition.h"
#include "model/relation/Relation.h"

class SingleCondition : public Condition {
private:
    Relation *rel;
    bool value;

public:
    SingleCondition(Relation *rel);
};
