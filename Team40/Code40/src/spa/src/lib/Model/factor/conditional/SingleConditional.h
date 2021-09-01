#pragma once
#include "../Conditional.h"
#include "Relational.h"

class SingleConditional : public Conditional {
private:
    Relational *rel;
    bool value;

public:
    SingleConditional(Relational *rel);
};
