#pragma once
#include "Expression.h"
#include "common/model/Term.h"

class SumTermsExpression : public Expression {
private:
    Term *term1;
    Term *term2;

public:
    SumTermsExpression(Term *term1, Term *term2);
};
