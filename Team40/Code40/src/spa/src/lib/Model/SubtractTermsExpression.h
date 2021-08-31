#pragma once
#include "Expression.h"
#include "Term.h"

class SubtractTermsExpression : public Expression {
    private:
    Term *term1;
    Term *term2;

    public:
    SubtractTermsExpression(Term *term1, Term *term2);
};
