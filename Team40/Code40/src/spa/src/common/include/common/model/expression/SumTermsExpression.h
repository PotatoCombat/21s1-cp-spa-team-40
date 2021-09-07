#pragma once
#include "Expression.h"
#include "model/term/Term.h"

class SumTermsExpression : public Expression {
private:
    Expression *expr;
    Term *term;

public:
    SumTermsExpression(Expression *expr, Term *term);
};
