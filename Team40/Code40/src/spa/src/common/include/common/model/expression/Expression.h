#pragma once
#include "model/Factor.h"

enum class ExpressionType {
    SINGLE_TERM,
    SUM_TERMS,
    SUBTRACT_TERMS,
};

class Expression : public Factor {
private:
    ExpressionType expressionType;
    bool hasComputedValue; // Used for lazy initialization of value

protected:
    // constructor

public:
    Expression(ExpressionType expressionType, int value);
    Expression(ExpressionType expressionType);

    // getters
    ExpressionType getExpressionType();
};
