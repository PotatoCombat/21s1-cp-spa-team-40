#pragma once
#include "../relation/Relation.h"
#include "Condition.h"

class OrCondition : public Condition {
private:
    Condition *cond1;
    Condition *cond2;

public:
    OrCondition(Condition *cond1, Condition *cond2);
};
