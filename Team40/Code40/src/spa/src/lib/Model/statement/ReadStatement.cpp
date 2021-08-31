#include "Statement.h"
#include "factor/Variable.h"

class ReadStatement : public Statement {
private:
    Variable variable;

public:
    ReadStatement::ReadStatement(int index, Variable variable)
        : variable(variable), Statement(index, StatementType::READ) {}

    Variable getVariable() { return this->variable; }
};
