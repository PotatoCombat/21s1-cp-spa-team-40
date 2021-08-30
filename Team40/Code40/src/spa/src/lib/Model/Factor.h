#pragma once
#include<vector>;
#include<string>;
using namespace std;

enum class FactorType {
	VARIABLE,
	CONSTANT,
	EXPRESSION
};

class Factor {
private:
	const FactorType factorType;

public:
	//constructor
	Factor(FactorType factorType);

	//getters
	const FactorType getFactorType();
	const virtual int getValue() = 0;
};
