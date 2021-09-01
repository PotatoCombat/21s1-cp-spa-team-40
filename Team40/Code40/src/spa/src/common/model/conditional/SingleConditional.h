#pragma once
#include "Conditional.h"
#include "common/model/relation/Relation.h"

class SingleConditional : public Conditional {
private:
    Relational *rel;
    bool value;

public:
    SingleConditional(Relational *rel);
};
