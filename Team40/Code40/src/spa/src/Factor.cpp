using namespace std;
#include<vector>;
#include<string>;
#include "Factor.h";

//constructor
Factor::Factor(string name) {
	this->name = name;
}

//setters
void Factor::setName(string name) {
	this->name = name;
}

void Factor::setValue(int value) {
	this->value = value;
}

void Factor::setType(FactorType type) {
	this->type = type;
}

//getters
string Factor::getName() {
	return this->name;
}

int Factor::getValue() {
	return this->value;
}

FactorType Factor::getFactorType() {
	return this->type;
}