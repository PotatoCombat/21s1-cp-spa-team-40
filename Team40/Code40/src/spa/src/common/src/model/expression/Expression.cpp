#include "common/model/expression/Expression.h"

Expression::Expression(ExpressionType expressionType)
    : expressionType(expressionType), Factor(FactorType::EXPRESSION) {}

ExpressionType Expression::getExpressionType() { return this->expressionType; }
