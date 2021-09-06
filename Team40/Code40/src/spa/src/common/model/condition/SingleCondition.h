#pragma once
#include "Condition.h"
#include "common/model/relation/Relation.h"

class SingleConditional : public Conditional {
private:
    Relation *rel;
    bool value;

public:
    SingleConditional(Relation *rel);
};
