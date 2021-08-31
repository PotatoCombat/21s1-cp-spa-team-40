#include "Statement.h"
#include "Variable.h"

class ReadStatement : public Statement {
private:
    Variable variable;

public:
    ReadStatement(int index, Variable variable)
        : variable(variable), Statement(index) {}

    Variable getVariable() { return this->variable; }
    StatementType getStatementType() { return StatementType::READ; }
};
