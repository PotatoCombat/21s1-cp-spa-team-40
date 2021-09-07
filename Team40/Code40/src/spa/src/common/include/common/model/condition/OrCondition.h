#pragma once
#include "Condition.h"
#include "common/model/relation/Relation.h"

class OrCondition : public Condition {
private:
    Condition *cond1;
    Condition *cond2;

public:
    OrCondition(Condition *cond1, Condition *cond2);
};