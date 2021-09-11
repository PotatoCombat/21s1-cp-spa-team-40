#include "common/model/statement/IfStatement.h"

IfStatement::IfStatement(int index, Condition *cond)
    : cond(cond), Statement(index, StatementType::IF) {
    thenStmtLst = {};
    elseStmtLst = {};
    condLst = {};
};

IfStatement::IfStatement(int index) : Statement(index, StatementType::IF) {
    this->thenStmtLst = {};
    this->elseStmtLst = {};
    this->condLst = {};
}

void IfStatement::addThenStatement(Statement *statement) {
    this->thenStmtLst.push_back(statement);
}

void IfStatement::addElseStatement(Statement *statement) {
    this->elseStmtLst.push_back(statement);
}

void IfStatement::setCondLst(vector<string> condLst) { condLst = condLst; }

Condition *IfStatement::getCondition() { return cond; }

vector<Statement *> IfStatement::getThenStmtLst() { return thenStmtLst; }

vector<Statement *> IfStatement::getElseStmtLst() { return elseStmtLst; }