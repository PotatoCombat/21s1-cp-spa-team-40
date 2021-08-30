
using namespace std;

#include "Statement.h"

Statement::Statement(int index) {
	this->StatementType = type;
}

int Statement::getIndex() {
	return index;
}

StatementType Statement::getStatementType() {
	return type;
}

vector<char> Statement::getVars() {
	return vars;
}

vector<int> Statement::getConsts() {
	return consts;
}