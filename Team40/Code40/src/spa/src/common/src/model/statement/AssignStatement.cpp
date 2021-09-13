#include "common/model/statement/AssignStatement.h"

AssignStatement::AssignStatement(int index, Variable variable)
    : variable(variable), Statement(index, StatementType::ASSIGN) {
    this->exprLst = {};
    this->varLst = {};
    this->constLst = {};
}

void AssignStatement::setExpressionLst(vector<string> exprLst) {
    this->exprLst = exprLst;
}

void AssignStatement::addExpressionVar(Variable var) {
    this->varLst.push_back(var);
}

void AssignStatement::addExpressionConst(ConstantValue constVal) {
    this->constLst.push_back(constVal);
}

Variable AssignStatement::getVariable() { return variable; }
