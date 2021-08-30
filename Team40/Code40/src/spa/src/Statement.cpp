
using namespace std;
#include<vector>;
#include "Statement.h"

//constructors
Statement::Statement(int index) {
	this->StatementType = type;
}

//setters
void Statement::setIndex(int index) {
	this->index = index;
}

void Statement::setStatementType(StatementType type) {
	this->type = type;
}

void Statement::addVar(char var) {
	this->varsList.push_back(var);
}

void Statement::addConst(int const) {
	this->constsList.push_back(const);
}

//getters
int Statement::getIndex() {
	return this->index;
}

StatementType Statement::getStatementType() {
	return this->type;
}

vector<char> Statement::getVars() {
	return this->varsList;
}

vector<int> Statement::getConsts() {
	return this->constsList;
}
