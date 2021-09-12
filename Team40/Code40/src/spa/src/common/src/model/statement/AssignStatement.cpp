#include "common/model/statement/AssignStatement.h"

AssignStatement::AssignStatement(int index, Variable *variable,
                                 Expression *expression)
    : variable(variable), expression(expression),
      Statement(index, StatementType::ASSIGN) {}

AssignStatement::AssignStatement(int index, Variable *variable)
    : variable(variable), Statement(index, StatementType::ASSIGN) {
    exprLst = {};
}
void AssignStatement::setExprLst(vector<string> exprLst) {
    this->exprLst = exprLst;
}

Variable *AssignStatement::getVariable() { return variable; }

Expression *AssignStatement::getExpression() { return expression; };