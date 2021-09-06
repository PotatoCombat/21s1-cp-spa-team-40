#include "SumTermsExpression.h"

SumTermsExpression::SumTermsExpression(Expression *expr, Term *term)
    : expr(expr), term(term),
      Expression(ExpressionType::SUM_TERMS,
                 expr->getValue() + term->getValue()) {}
