#include "common/model/Variable.h"
#include "common/model/statement/Statement.h"
#include <string>
#include <vector>

class AssignStatement : public Statement {
private:
    Variable variable;
    // No AST yet
    // Expression expression;
    vector<string> exprLst;

public:
    // AssignStatement(int index, Variable variable, Expression expression)
    //     : variable(variable), expression(expression),
    //       Statement(index, StatementType::ASSIGN) {}

    AssignStatement(int index, Variable variable)
        : variable(variable), Statement(index, StatementType::ASSIGN) {
        exprLst = {};
    }

    void setExprLst(vector<string> exprLst) { this->exprLst = exprLst; }

    Variable getVariable() { return this->variable; }

    // Expression getExpression() { return this->expression; }

    StatementType getStatementType() { return StatementType::ASSIGN; }
};
