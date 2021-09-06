#pragma once
#include "Conditional.h"
#include "common/model/relation/Relation.h"

class SingleConditional : public Conditional {
private:
    Relation *rel;
    bool value;

public:
    SingleConditional(Relation *rel);
};
