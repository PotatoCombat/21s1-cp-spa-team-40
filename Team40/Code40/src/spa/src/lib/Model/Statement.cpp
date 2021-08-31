#include "Statement.h"
#include <vector>

// constructors
Statement::Statement(int index) { this->index = index; }

// setters
void Statement::setStatementType(StatementType statementType) {
  this->statementType = statementType;
}

void Statement::addVar(char var) { this->varsList.push_back(var); }

void Statement::addConst(int constant) { this->constsList.push_back(constant); }

// getters
int Statement::getIndex() { return this->index; }

StatementType Statement::getStatementType() { return this->statementType; }

vector<char> Statement::getVars() { return this->varsList; }

vector<int> Statement::getConsts() { return this->constsList; }
