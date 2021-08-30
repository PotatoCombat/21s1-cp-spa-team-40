#pragma once
#include "Factor.h"

enum class ExpressionType {
	SINGLE_TERM,
    SUM_OF_TERMS,
    DIFFERENCE_OF_TERMS
};

class Expression : public Factor {
private:
	ExpressionType expressionType;
public:
	//constructor
	Expression(ExpressionType expressionType, int value);

	//getters
	ExpressionType getExpressionType();

	virtual int evaluateSelf() = 0;
};
