#include "common/model/statement/IfStatement.h"

IfStatement::IfStatement(int index) : Statement(index, StatementType::IF) {
    this->thenStmtLst = {};
    this->elseStmtLst = {};
    this->condLst = {};
    this->varLst = {};
    this->constLst = {};
}

void IfStatement::addThenStatement(Statement *statement) {
    this->thenStmtLst.push_back(statement);
}

void IfStatement::addElseStatement(Statement *statement) {
    this->elseStmtLst.push_back(statement);
}

void IfStatement::setCondLst(vector<string> condLst) {
    this->condLst = condLst;
}

void IfStatement::addVar(Variable var) { this->varLst.push_back(var); }

void IfStatement::addConst(ConstantValue constVal) {
    this->constLst.push_back(constVal);
}

vector<Statement *> IfStatement::getThenStmtLst() { return thenStmtLst; }

vector<Statement *> IfStatement::getElseStmtLst() { return elseStmtLst; }