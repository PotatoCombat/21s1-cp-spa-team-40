#include "Statement.h"
#include "lib/Model/Factor.h"
#include "lib/Model/Variable.h"

class PrintStatement : public Statement {

private:
	Variable variable;

public:
    PrintStatement(int index, Variable variable): variable(variable), Statement(index) {}

	Variable getVariable() {
		return this->variable;
	}
	StatementType getStatementType() {
		return StatementType::PRINT;
	}
};