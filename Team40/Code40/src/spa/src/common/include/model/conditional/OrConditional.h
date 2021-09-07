#pragma once
#include "Conditional.h"
#include "model/relation/Relation.h"

class OrConditional : public Conditional {
private:
    Conditional *cond1;
    Conditional *cond2;

public:
    OrConditional(Conditional *cond1, Conditional *cond2);
};
