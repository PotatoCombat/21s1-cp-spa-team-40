#pragma once
#include "common/model/Factor.h"

enum class RelationType {
    MORE_THAN,
    MORE_THAN_OR_EQUALS,
    LESS_THAN,
    LESS_THAN_OR_EQUALS,
    EQUALS,
    NOT_EQUALS
};

class Relation {
private:
    RelationType relType;
    Factor leftFactor;
    Factor rightFactor;
    bool value;

public:
    // constructor
    Relation(RelationType relType, Factor leftFactor, Factor rightFactor);

    // getters
    RelationType getRelationType();
    virtual bool getValue() = 0;
    Factor getLeftFactor();
    Factor getRightFactor();
};
