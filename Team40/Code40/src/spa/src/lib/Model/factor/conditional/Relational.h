#pragma once
#include "Factor.h"

enum class RelationalExpressionType {
    MORE_THAN,
    MORE_THAN_OR_EQUALS,
    LESS_THAN,
    LESS_THAN_OR_EQUALS,
    EQUALS,
    NOT_EQUALS
};

class RelationalExpression {
private:
    RelationalExpressionType relExprType;
    Factor fac1;
    Factor fac2;

public:
    // constructor
    RelationalExpression(RelationalExpressionType relExprType, Factor fac1,
                         Factor fac2);

    // getters
    RelationalExpressionType getRelExprType();
};
