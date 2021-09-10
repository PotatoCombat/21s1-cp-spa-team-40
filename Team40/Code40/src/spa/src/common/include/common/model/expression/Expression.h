#pragma once
#include "common/model/Factor.h"
#include "common/model/term/Term.h"

enum class ExpressionType {
    SINGLE_TERM,
    SUM_TERMS,
    SUBTRACT_TERMS,
};

class Expression : public Factor {
private:
    ExpressionType expressionType;

protected:
    explicit Expression(ExpressionType expressionType);

public:
    // getters
    ExpressionType getExpressionType();
};
