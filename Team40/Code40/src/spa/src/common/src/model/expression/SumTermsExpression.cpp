#include "common/model/expression/SumTermsExpression.h"

SumTermsExpression::SumTermsExpression(Expression *expr, Term *term)
    : expr(expr), term(term), Expression(ExpressionType::SUM_TERMS) {}
