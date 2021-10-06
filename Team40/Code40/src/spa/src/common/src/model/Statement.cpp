#include "common/model/Statement.h"

Statement::Statement(int index, StatementType statementType) {
    this->index = index;
    this->statementType = statementType;
    this->expressionVars = {};
    this->expressionConsts = {};
}

int Statement::getIndex() { return index; }

StatementType Statement::getStatementType() { return statementType; }

Variable *Statement::getVariable() {
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

set<Variable *, Comparator<Variable>> Statement::getExpressionVars() { return expressionVars; };

set<ConstantValue *, Comparator<ConstantValue>> Statement::getExpressionConsts() {
    return expressionConsts;
};

vector<Statement *> Statement::getThenStmtLst() {
    if (!thenStmtLst.has_value()) {
        throw runtime_error("This statement does not use a THEN statement list.");
    }
    return thenStmtLst.value();
}

vector<Statement *> Statement::getElseStmtLst() {
    if (!elseStmtLst.has_value()) {
        throw runtime_error("This statement does not use an ELSE statement list.");
    }
    return elseStmtLst.value();
}

void Statement::setVariable(Variable *variable) { this->variable = variable; }

void Statement::setProcName(ProcName procName) { this->procName = procName; }

void Statement::setExpressionLst(vector<string> expressionLst) {
    this->expressionLst = expressionLst;
}

void Statement::addExpressionVar(Variable *expressionVar) { expressionVars.insert(expressionVar); }

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

bool Statement::operator==(const Statement &other) const {
    bool sameIndex = index == other.index;
    bool sameType = statementType == other.statementType;

    bool sameVar = variable.has_value() == other.variable.has_value();
    if (variable.has_value() && other.variable.has_value()) {
        sameVar = *variable.value() == *other.variable.value();
    }

    bool sameProc = procName.has_value() == other.procName.has_value();
    if (procName.has_value() && other.procName.has_value()) {
        sameProc = procName.value() == other.procName.value();
    }

    bool sameExpressionLst = expressionLst.has_value() == other.expressionLst.has_value();
    if (expressionLst.has_value() && other.expressionLst.has_value()) {
        sameExpressionLst = expressionLst.value() == other.expressionLst.value();
    }

    bool sameExpressionVars =
        expressionVars.size() == other.expressionVars.size() &&
        equal(expressionVars.begin(), expressionVars.end(), other.expressionVars.begin(),
              [](const Variable *lhs, const Variable *rhs) { return *lhs == *rhs; });

    bool sameExpressionConsts =
        expressionConsts.size() == other.expressionConsts.size() &&
        equal(expressionConsts.begin(), expressionConsts.end(), other.expressionConsts.begin(),
              [](const ConstantValue *lhs, const ConstantValue *rhs) { return *lhs == *rhs; });

    bool sameThenStmtList = thenStmtLst.has_value() == other.thenStmtLst.has_value();
    if (thenStmtLst.has_value() && other.thenStmtLst.has_value()) {
        sameThenStmtList =
            equal(thenStmtLst->begin(), thenStmtLst->end(), other.thenStmtLst->begin(),
                  [](const Statement *lhs, const Statement *rhs) { return *lhs == *rhs; });
    }

    bool sameElseStmtList = elseStmtLst.has_value() == other.elseStmtLst.has_value();
    if (elseStmtLst.has_value() && other.thenStmtLst.has_value()) {
        sameThenStmtList =
            equal(elseStmtLst->begin(), elseStmtLst->end(), other.elseStmtLst->begin(),
                  [](const Statement *lhs, const Statement *rhs) { return *lhs == *rhs; });
    }

    return sameIndex && sameType && sameVar && sameProc && sameExpressionLst &&
           sameExpressionVars && sameExpressionConsts && sameThenStmtList && sameElseStmtList;
}
