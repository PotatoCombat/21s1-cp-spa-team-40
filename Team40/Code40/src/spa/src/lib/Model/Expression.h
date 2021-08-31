#pragma once
#include "Factor.h"

enum class ExpressionType {
    SINGLE_TERM,
    SUM_TERMS,
    SUBTRACT_TERMS,
};

class Expression : public Factor {
    private:
    ExpressionType expressionType;

    public:
    // constructor
    Expression(ExpressionType expressionType, int value);

    // getters
    ExpressionType getExpressionType();
};
