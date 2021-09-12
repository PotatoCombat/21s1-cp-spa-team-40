#pragma once
#include "Expression.h"
#include "common/model/term/Term.h"

class SumTermsExpression : public Expression {
private:
    Expression *expr;
    Term *term;

public:
    SumTermsExpression(Expression *expr, Term *term);

    Expression* getExpression();

    Term* getTerm();
};
