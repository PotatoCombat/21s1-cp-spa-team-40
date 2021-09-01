#pragma once
#include "../Conditional.h"
#include "Relational.h"

class AndConditional : public Conditional {
private:
    Conditional *cond1;
    Conditional *cond2;

public:
    AndConditional(Conditional *cond1, Conditional *cond2);
};
