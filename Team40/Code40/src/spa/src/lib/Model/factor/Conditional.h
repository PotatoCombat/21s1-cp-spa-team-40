#pragma once

enum class ConditionalExpressionType { SINGLE, AND, OR, NOT };

class ConditionalExpression {
private:
    ConditionalExpressionType condExprType;
    // RelationalExpression relExpr;      // for SINGLE only
    // ConditionalExpression condExpr1; // for NOT, AND and OR
    // ConditionalExpression condExpr2; // for AND and OR only

public:
    // constructor
    ConditionalExpression(ConditionalExpressionType condExprType);

    // getters
    ConditionalExpressionType getCondExprType();
};
