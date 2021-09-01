#pragma once
#include "../Conditional.h"
#include "Relational.h"

class OrConditional : public Conditional {
private:
    Conditional *cond1;
    Conditional *cond2;

public:
    OrConditional(Conditional *cond1, Conditional *cond2);
};
