#include "common/model/statement/WhileStatement.h"

WhileStatement::WhileStatement(int index)
    : Statement(index, StatementType::WHILE) {
    this->stmtLst = {};
    this->condLst = {};
    this->varLst = {};
    this->constLst = {};
}

void WhileStatement::addStatement(Statement statement) {
    this->stmtLst.push_back(statement);
}

void WhileStatement::setExpressionLst(vector<string> condLst) {
    this->condLst = condLst;
}

void WhileStatement::addExpressionVar(Variable var) {
    this->varLst.push_back(var);
}

void WhileStatement::addExpressionConst(ConstantValue constVal) {
    this->constLst.push_back(constVal);
}

vector<Statement> WhileStatement::getThenStmtLst() { return stmtLst; }