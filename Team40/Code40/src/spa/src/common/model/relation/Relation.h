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
    bool value;

public:
    // constructor
    Relation(RelationType relType, bool value);

    // getters
    RelationType getRelationType();
    bool getValue();
};
