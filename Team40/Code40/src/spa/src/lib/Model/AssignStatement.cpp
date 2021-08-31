#include "Expression.h"
#include "Statement.h"
#include "Variable.h"

class AssignStatement : public Statement {
private:
    Variable variable;
    Expression expression;

public:
    AssignStatement(int index, Variable variable, Expression expression)
        : variable(variable), expression(expression), Statement(index) {}

    Variable getVariable() { return this->variable; }

    Expression getExpression() { return this->expression; }

    StatementType getStatementType() { return StatementType::ASSIGN; }
};
