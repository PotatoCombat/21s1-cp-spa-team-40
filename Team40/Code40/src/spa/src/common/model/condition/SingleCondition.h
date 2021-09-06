#pragma once
#include "../relation/Relation.h"
#include "Condition.h"

class SingleCondition : public Condition {
private:
    Relation *rel;
    bool value;

public:
    SingleCondition(Relation *rel);
};
