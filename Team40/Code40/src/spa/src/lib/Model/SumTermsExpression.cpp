#include "SumTermsExpression.h"

SumTermsExpression::SumTermsExpression(Term *term1, Term *term2):
    term1(term1),
    term2(term2),
    Expression(
        ExpressionType::SUM_TERMS,
        term1->getValue() + term2->getValue()
    ) {}