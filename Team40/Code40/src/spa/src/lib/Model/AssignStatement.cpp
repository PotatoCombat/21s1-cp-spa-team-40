#include "Statement.h"
#include "Variable.h"

class AssignStatement : public Statement {
private:
  Variable variable;

public:
  AssignStatement(int index, Variable variable)
      : variable(variable), Statement(index) {}

  Variable getVariable() { return this->variable; }

  StatementType getStatementType() { return StatementType::ASSIGN; }
};
