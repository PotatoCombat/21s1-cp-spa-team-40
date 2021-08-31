#include "Expression.h"

Expression::Expression(ExpressionType expressionType, int value): expressionType(expressionType), Factor(FactorType::EXPRESSION, value) {}

ExpressionType Expression::getExpressionType() {
    return this->expressionType;
}