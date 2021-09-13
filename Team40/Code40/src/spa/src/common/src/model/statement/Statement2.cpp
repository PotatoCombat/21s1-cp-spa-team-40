#include "common/model/statement/Statement2.h"

Statement2::Statement2(int index, StatementType statementType) {
    this->index = index;
    this->statementType = statementType;
}

int Statement2::getIndex() { return index; }

StatementType Statement2::getStatementType() { return statementType; }

Variable Statement2::getVariable() {
    if (!variable.has_value()) {
        throw runtime_error("This statement does not use a variable.");
    }
    return variable.value();
};

Procedure Statement2::getProcedure() {
    if (!procedure.has_value()) {
        throw runtime_error("This statement does not use a procedure.");
    }
    return procedure.value();
};

vector<string> Statement2::getExpressionLst() {
    if (!expressionLst.has_value()) {
        throw runtime_error("This statement does not use an expression.");
    }
    return expressionLst.value();
};

vector<Variable> Statement2::getExpressionVars() {
    if (!expressionVars.has_value()) {
        throw runtime_error(
            "This statement does not use an expression (with variables).");
    }
    return expressionVars.value();
};

vector<ConstantValue> Statement2::getExpressionConsts() {
    if (!expressionConsts.has_value()) {
        throw runtime_error(
            "This statement does not use an expression (with constants).");
    }
    return expressionConsts.value();
};

vector<Statement2> Statement2::getThenStmtLst() {
    if (!thenStmtLst.has_value()) {
        throw runtime_error(
            "This statement does not use a THEN statement list.");
    }
    return thenStmtLst.value();
}

vector<Statement2> Statement2::getElseStmtLst() {
    if (!elseStmtLst.has_value()) {
        throw runtime_error(
            "This statement does not use an ELSE statement list.");
    }
    return elseStmtLst.value();
}

void Statement2::setVariable(Variable variable) {
    this->variable = variable;
}

void Statement2::setProcedure(Procedure procedure) {
   this->procedure = procedure;
}

void Statement2::setExpressionLst(vector<string> expressionLst) {
    this->expressionLst = expressionLst;
}

void Statement2::addExpressionVar(Variable expressionVar) {
    if (!this->expressionVars.has_value()) {
        this->expressionVars = {expressionVar};
        return;
    }
    this->expressionVars->push_back(expressionVar);
}

void Statement2::addExpressionConst(ConstantValue expressionConst) {
    if (!this->expressionConsts.has_value()) {
        this->expressionConsts = {expressionConst};
        return;
    }
    this->expressionConsts->push_back(expressionConst);
}

void Statement2::addThenStmt(Statement2 stmt) {
    if (!this->thenStmtLst.has_value()) {
        this->thenStmtLst = {stmt};
        return;
    }
    this->thenStmtLst->push_back(stmt);
}

void Statement2::addElseStmt(Statement2 stmt) {
    if (!this->elseStmtLst.has_value()) {
        this->elseStmtLst = {stmt};
        return;
    }
    this->elseStmtLst->push_back(stmt);
}
