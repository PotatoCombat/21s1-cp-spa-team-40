#pragma once
#include "Conditional.h"
#include "model/relation/Relation.h"

class NotConditional : public Conditional {
private:
    Conditional *cond;

public:
    NotConditional(Conditional *cond);
};
