#pragma once
#include "../Conditional.h"
#include "Relational.h"

class SingleConditional : public Conditional {
private:
    Conditional *cond;

public:
    SingleConditional(Conditional *cond);
};
