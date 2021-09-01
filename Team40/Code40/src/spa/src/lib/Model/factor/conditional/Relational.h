#pragma once
#include "../Factor.h"

enum class RelationalType {
    MORE_THAN,
    MORE_THAN_OR_EQUALS,
    LESS_THAN,
    LESS_THAN_OR_EQUALS,
    EQUALS,
    NOT_EQUALS
};

class Relational {
private:
    RelationalType relType;
    Factor fac1;
    Factor fac2;

public:
    // constructor
    Relational(RelationalType relType, Factor fac1,
                         Factor fac2);

    // getters
    RelationalType getRelType();
};
