using namespace std;
#include "Statment.h"
#include "Factor.h"
using Statement::Statement;
using Factor::Factor;

class ReadStatement : public Statement {

private:
	Factor factor;

public:
	void setFactor(Factor factor) { //this can only be a variable
		this->factor = factor;
	}

	Factor getFactor() {
		return this->getFactor;
	}
	StatementType getStatementType() {
		return StatementType::READ;
	}
};