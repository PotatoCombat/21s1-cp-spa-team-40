#pragma once
#include "../term/Term.h"
#include "Expression.h"


class SumTermsExpression : public Expression {
private:
    Expression *expr;
    Term *term;

public:
    SumTermsExpression(Expression *expr, Term *term);
};
