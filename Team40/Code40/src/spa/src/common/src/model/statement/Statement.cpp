#include "common/model/statement/Statement.h"

Statement::Statement(int index, StatementType statementType) {
    this->index = index;
    this->statementType = statementType;
}

int Statement::getIndex() { return index; }

StatementType Statement::getStatementType() { return statementType; }

Variable Statement::getVariable() {
    if (!variable.has_value()) {
        throw runtime_error("This statement does not use a variable.");
    }
    return variable.value();
};

Procedure Statement::getProcedure() {
    if (!procedure.has_value()) {
        throw runtime_error("This statement does not use a procedure.");
    }
    return procedure.value();
};

vector<string> Statement::getExpressionLst() {
    if (!expressionLst.has_value()) {
        throw runtime_error("This statement does not use an expression.");
    }
    return expressionLst.value();
};

vector<Variable> Statement::getExpressionVars() {
    if (!expressionVars.has_value()) {
        throw runtime_error(
            "This statement does not use an expression (with variables).");
    }
    return expressionVars.value();
};

vector<ConstantValue> Statement::getExpressionConsts() {
    if (!expressionConsts.has_value()) {
        throw runtime_error(
            "This statement does not use an expression (with constants).");
    }
    return expressionConsts.value();
};

vector<Statement> Statement::getThenStmtLst() {
    if (!thenStmtLst.has_value()) {
        throw runtime_error(
            "This statement does not use a THEN statement list.");
    }
    return thenStmtLst.value();
}

vector<Statement> Statement::getElseStmtLst() {
    if (!elseStmtLst.has_value()) {
        throw runtime_error(
            "This statement does not use an ELSE statement list.");
    }
    return elseStmtLst.value();
}

void Statement::setVariable(Variable variable) { this->variable = variable; }

void Statement::setProcedure(Procedure procedure) {
    this->procedure = procedure;
}

void Statement::setExpressionLst(vector<string> expressionLst) {
    this->expressionLst = expressionLst;
}

void Statement::addExpressionVar(Variable expressionVar) {
    if (!this->expressionVars.has_value()) {
        this->expressionVars = {expressionVar};
        return;
    }
    this->expressionVars->push_back(expressionVar);
}

void Statement::addExpressionConst(ConstantValue expressionConst) {
    if (!this->expressionConsts.has_value()) {
        this->expressionConsts = {expressionConst};
        return;
    }
    this->expressionConsts->push_back(expressionConst);
}

void Statement::addThenStmt(Statement stmt) {
    if (!this->thenStmtLst.has_value()) {
        this->thenStmtLst = {stmt};
        return;
    }
    this->thenStmtLst->push_back(stmt);
}

void Statement::addElseStmt(Statement stmt) {
    if (!this->elseStmtLst.has_value()) {
        this->elseStmtLst = {stmt};
        return;
    }
    this->elseStmtLst->push_back(stmt);
}
