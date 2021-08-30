using namespace std;
#include "Statement.h"
#include "Factor.h"

class PrintStatement : public Statement {

private:
	Factor variable;
	//Expression expression; //TODO: Implement this

public:
	void setVariable(Factor variable) { //this can only be a factor of type variable
		this->variable = variable;
	}

	Factor getVariable() {
		return this->variable;
	}
	StatementType getStatementType() {
		return StatementType::ASSIGN;
	}
};