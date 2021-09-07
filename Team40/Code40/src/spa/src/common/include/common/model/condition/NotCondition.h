#pragma once
#include "Condition.h"
#include "common/model/relation/Relation.h"

class NotCondition : public Condition {
private:
    Condition *cond;

public:
    NotCondition(Condition *cond);
};
