#include "common/model/Statement.h"

Statement::Statement(int index, StatementType statementType) {
    this->index = index;
    this->statementType = statementType;
    this->expressionVars = {};
    this->expressionConsts = {};
}

int Statement::getIndex() { return index; }

StatementType Statement::getStatementType() { return statementType; }

Variable* Statement::getVariable() {
    if (!variable.has_value()) {
        throw runtime_error("This statement does not use a variable.");
    }
    return variable.value();
};

ProcName Statement::getProcName() {
    if (!procName.has_value()) {
        throw runtime_error("This statement does not use a procedure.");
    }
    return procName.value();
};

vector<string> Statement::getExpressionLst() {
    if (!expressionLst.has_value()) {
        throw runtime_error("This statement does not use an expression.");
    }
    return expressionLst.value();
};

set<Variable *, Comparator<Variable>> Statement::getExpressionVars() {
    return expressionVars;
};

set<ConstantValue *, Comparator<ConstantValue>> Statement::getExpressionConsts() {
    return expressionConsts;
};

vector<Statement *> Statement::getThenStmtLst() {
    if (!thenStmtLst.has_value()) {
        throw runtime_error(
            "This statement does not use a THEN statement list.");
    }
    return thenStmtLst.value();
}

vector<Statement *> Statement::getElseStmtLst() {
    if (!elseStmtLst.has_value()) {
        throw runtime_error(
            "This statement does not use an ELSE statement list.");
    }
    return elseStmtLst.value();
}

void Statement::setVariable(Variable *variable) { this->variable = variable; }

void Statement::setProcName(ProcName procName) { this->procName = procName; }

void Statement::setExpressionLst(vector<string> expressionLst) {
    this->expressionLst = expressionLst;
}

void Statement::addExpressionVar(Variable *expressionVar) {
    expressionVars.insert(expressionVar);
}

void Statement::addExpressionConst(ConstantValue *expressionConst) {
    expressionConsts.insert(expressionConst);
}

void Statement::addThenStmt(Statement *stmt) {
    if (!this->thenStmtLst.has_value()) {
        this->thenStmtLst = {stmt};
        return;
    }
    this->thenStmtLst->push_back(stmt);
}

void Statement::addElseStmt(Statement *stmt) {
    if (!this->elseStmtLst.has_value()) {
        this->elseStmtLst = {stmt};
        return;
    }
    this->elseStmtLst->push_back(stmt);
}
