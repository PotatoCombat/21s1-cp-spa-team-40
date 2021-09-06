#include "../Variable.h"
#include "../statement/Statement.h"

class ReadStatement : public Statement {
private:
    Variable variable;

public:
    ReadStatement(int index, Variable variable)
        : variable(variable), Statement(index, StatementType::READ) {}

    Variable getVariable() { return this->variable; }
};
