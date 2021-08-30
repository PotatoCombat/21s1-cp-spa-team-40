using namespace std;
#include "Statement.h";
#include "Factor.h";
#include "Expression.h";

class PrintStatement : public Statement {

private:
	Factor variable;
	Expression expression;

public:
	void setVariable(Factor variable) { //this can only be a factor of type variable
		this->variable = variable;
	}

	void setExpression(Expression expression) {
		this->expression = expression;
	}

	Factor getVariable() {
		return this->variable;
	}

	Expression getExpression() {
		return this->expression;
	}

	StatementType getStatementType() {
		return StatementType::ASSIGN;
	}
};