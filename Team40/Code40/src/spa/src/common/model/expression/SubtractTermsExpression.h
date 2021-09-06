#pragma once
#include "../term/Term.h"
#include "Expression.h"

class SubtractTermsExpression : public Expression {
private:
    Expression *expr;
    Term *term;

public:
    SubtractTermsExpression(Expression *expr, Term *term);
};
