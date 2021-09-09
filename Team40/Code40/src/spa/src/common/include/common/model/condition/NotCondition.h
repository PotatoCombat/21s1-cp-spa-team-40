#pragma once
#include "Condition.h"
#include "common/model/relation/Relation.h"

class NotCondition : public Condition {
private:
    Condition *condition;

public:
    explicit NotCondition(Condition *condition);
    Condition *getPrimaryCondition() override;
};
