#pragma once

enum class FactorType {
	VARIABLE,
	CONSTANT,
	EXPRESSION
};

class Factor {
private:
	const FactorType factorType;
	const int value;

public:
	//constructor
	explicit Factor(FactorType factorType);

	//getters
	const FactorType getFactorType();

    virtual int getValue() = 0;
};
