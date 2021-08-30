#pragma once
#include "Factor.h";
using namespace std;

enum class ExpressionType {
	SINGLE_TERM,
    SUM_OF_TERMS,
    DIFF_OF_TERMS
};

class Expression : public Factor{
private:
	ExpressionType expressionType;
public:
	//constructor
	Expression(string name);

    //setters
	void setExpressionType(ExpressionType expressionType);

	//getters
	ExpressionType getExpressionType();
};
