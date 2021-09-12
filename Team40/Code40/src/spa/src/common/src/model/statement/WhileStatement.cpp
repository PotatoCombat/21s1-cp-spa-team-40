#include "common/model/statement/WhileStatement.h"

WhileStatement::WhileStatement(int index)
    : Statement(index, StatementType::WHILE) {
    this->stmtLst = {};
    this->condLst = {};
}

WhileStatement::WhileStatement(int index, Condition *cond)
    : cond(cond), Statement(index, StatementType::WHILE) {
    this->stmtLst = {};
}

void WhileStatement::addStatement(Statement *statement) {
    this->stmtLst.push_back(statement);
}

void WhileStatement::setCondLst(vector<string> condLst) {
    this->condLst = condLst;
}

Condition *WhileStatement::getCondition() { return cond; }

vector<Statement *> WhileStatement::getThenStmtLst() { return stmtLst; }