#pragma once
#include "Condition.h"
#include "model/relation/Relation.h"

class AndCondition : public Condition {
private:
    Condition *cond1;
    Condition *cond2;

public:
    AndCondition(Condition *cond1, Condition *cond2);
};
