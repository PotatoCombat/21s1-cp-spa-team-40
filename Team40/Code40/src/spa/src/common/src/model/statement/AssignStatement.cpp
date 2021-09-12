#include <utility>
#include "common/model/statement/AssignStatement.h"

using namespace std;

AssignStatement::AssignStatement(int index, Variable *variable)
    : variable(variable), Statement(index, StatementType::ASSIGN) {};

void AssignStatement::setExprLst(vector<string> exprLst) {
    this->exprLst = move(exprLst);
}

Variable *AssignStatement::getVariable() { return variable; }

vector<string> AssignStatement::getExprList() { return exprLst; };