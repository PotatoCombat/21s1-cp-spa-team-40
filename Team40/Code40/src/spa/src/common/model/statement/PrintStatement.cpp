#include "Statement.h"
#include "factor/Variable.h"

class PrintStatement : public Statement {
private:
    Variable variable;

public:
    PrintStatement::PrintStatement(int index, Variable variable)
        : variable(variable), Statement(index, StatementType::PRINT) {}

    Variable getVariable() { return this->variable; }
};
