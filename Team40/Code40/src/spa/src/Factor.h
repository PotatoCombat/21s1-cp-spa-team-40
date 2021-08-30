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
	string name;
	int value;
	FactorType factorType;

public:
	//constructor
	Factor(string name);

	//setters
	void setName(string name);
	void setValue(int value);
	void setFactorType(FactorType factorType);

	//getters
	string getName();
	int getValue();
	FactorType getFactorType();
};
