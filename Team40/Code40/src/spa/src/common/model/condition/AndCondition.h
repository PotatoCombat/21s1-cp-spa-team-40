#pragma once
#include "Condition.h"
#include "common/model/relation/Relation.h"

class AndConditional : public Conditional {
private:
    Conditional *cond1;
    Conditional *cond2;

public:
    AndConditional(Conditional *cond1, Conditional *cond2);
};
