#pragma once
#include "Conditional.h"
#include "model/relation/Relation.h"

class SingleConditional : public Conditional {
private:
    Relation *rel;
    bool value;

public:
    SingleConditional(Relation *rel);
};
