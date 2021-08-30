using namespace std;
#include "Statment.h
#include "Factor.h"
using Statement::Statement;
using Factor::Factor;

class PrintStatement : public Statement {

private:
	Factor variable;
	Expression expression;

public:
	void setVariable(Factor variable) { //this can only be a variable
		this->variable = variable;
	}

	Factor getVariable() {
		return this->variable;
	}
	StatementType getStatementType() {
		return StatementType::READ;
	}
};