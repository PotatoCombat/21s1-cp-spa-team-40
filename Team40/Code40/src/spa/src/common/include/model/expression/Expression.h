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

protected:
    // constructor
    Expression(ExpressionType expressionType, int value);

public:
    // getters
    ExpressionType getExpressionType();
};
