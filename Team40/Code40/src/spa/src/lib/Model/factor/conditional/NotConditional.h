#pragma once
#include "../Conditional.h"
#include "Relational.h"

class NotConditional : public Conditional {
private:
    Conditional *cond;

public:
    NotConditional(Conditional *cond);
};
