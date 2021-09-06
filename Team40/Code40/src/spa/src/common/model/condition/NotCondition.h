#pragma once
#include "../relation/Relation.h"
#include "Condition.h"

class NotCondition : public Condition {
private:
    Condition *cond;

public:
    NotCondition(Condition *cond);
};
