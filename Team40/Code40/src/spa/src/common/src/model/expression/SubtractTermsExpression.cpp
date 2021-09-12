#include "common/model/expression/SubtractTermsExpression.h"

SubtractTermsExpression::SubtractTermsExpression(Expression *expr, Term *term)
    : expr(expr), term(term), Expression(ExpressionType::SUBTRACT_TERMS) {}

Term *SubtractTermsExpression::getTerm() { return term; }

Expression* SubtractTermsExpression::getExpression() { return expr; }