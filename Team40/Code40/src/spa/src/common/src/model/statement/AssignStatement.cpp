#include "common/model/statement/AssignStatement.h"

AssignStatement::AssignStatement(int index, Variable *variable)
    : variable(variable), Statement(index, StatementType::ASSIGN) {
    this->exprLst = {};
    this->varLst = {};
    this->constLst = {};
}

void AssignStatement::setExprLst(vector<string> exprLst) {
    this->exprLst = exprLst;
}

void AssignStatement::addVar(Variable var) { this->varLst.push_back(var); }

void AssignStatement::addConst(ConstantValue constVal) {
    this->constLst.push_back(constVal);
}

Variable *AssignStatement::getVariable() { return variable; }
