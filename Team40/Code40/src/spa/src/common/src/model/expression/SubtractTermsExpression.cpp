#include "model/expression/SubtractTermsExpression.h"

SubtractTermsExpression::SubtractTermsExpression(Expression *expr, Term *term)
    : expr(expr), term(term), Expression(ExpressionType::SUBTRACT_TERMS,
                                         expr->getValue() - term->getValue()) {}
