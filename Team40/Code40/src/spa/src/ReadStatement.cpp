using namespace std;
#include "Statement.h"
#include "Factor.h"

class ReadStatement : public Statement {

private:
	Factor variable;

public:
	void setVariable(Factor variable) { //this can only be a factor of type variable
		this->variable = variable;
	}

	Factor getVariable() {
		return this->variable;
	}
	StatementType getStatementType() {
		return StatementType::READ;
	}
};