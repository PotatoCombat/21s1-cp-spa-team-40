#pragma once
#include "common/model/Factor.h"

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
    explicit Expression(ExpressionType expressionType);

    // getters
    ExpressionType getExpressionType();
};
