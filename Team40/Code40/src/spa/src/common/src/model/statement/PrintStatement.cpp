#include "Statement.h"
#include "Variable.h"

class PrintStatement : public Statement {
private:
    Variable variable;

public:
    PrintStatement(int index, Variable variable)
        : variable(variable), Statement(index, StatementType::PRINT) {}

    Variable getVariable() { return this->variable; }
};
