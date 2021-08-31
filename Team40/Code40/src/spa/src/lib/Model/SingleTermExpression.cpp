#include "SingleTermExpression.h"

SingleTermExpression::SingleTermExpression(Term *term)
    : term(term), Expression(ExpressionType::SINGLE_TERM, term->getValue()) {}
