#pragma once
#include "Expression.h"
#include "common/model/term/Term.h"

class SubtractTermsExpression : public Expression {
private:
    Expression *expr;
    Term *term;

public:
    SubtractTermsExpression(Expression *expr, Term *term);

    Expression* getExpression();

    Term* getTerm();
};
