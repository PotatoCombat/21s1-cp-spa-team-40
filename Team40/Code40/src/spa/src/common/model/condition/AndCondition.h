#pragma once
#include "../relation/Relation.h"
#include "Condition.h"

class AndCondition : public Condition {
private:
    Condition *cond1;
    Condition *cond2;

public:
    AndCondition(Condition *cond1, Condition *cond2);
};
