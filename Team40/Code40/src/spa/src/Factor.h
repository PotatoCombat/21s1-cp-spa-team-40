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
	FactorType type;

public:
	//constructor
	Factor(string name);

	//setters
	void setName(string name);
	void setValue(int value);
	void setType(FactorType type);

	//getters
	string getName();
	int getValue();
	FactorType getType();
};
