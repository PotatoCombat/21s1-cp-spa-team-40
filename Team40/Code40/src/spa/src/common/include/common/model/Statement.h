#pragma once

#include "common/model/Abstractions.h"
#include "common/model/ConstantValue.h"
#include "common/model/Variable.h"


#include <optional>
#include <set>
#include <stdexcept>
#include <vector>

enum class StatementType { UNKNOWN, READ, PRINT, ASSIGN, CALL, WHILE, IF };

class Statement {
public:
    Statement(int index, StatementType statementType);

    int getIndex();
    StatementType getStatementType();

    Variable* getVariable();
    ProcName getProcName();

    vector<string> getExpressionLst();
    set<Variable *> getExpressionVars();
    set<ConstantValue *> getExpressionConsts();

    vector<Statement *> getThenStmtLst();
    vector<Statement *> getElseStmtLst();

    void setVariable(Variable *variable);
    void setProcName(ProcName procName);

    void setExpressionLst(vector<string> expressionLst);
    void addExpressionVar(Variable *expressionVar);
    void addExpressionConst(ConstantValue *expressionConst);

    void addThenStmt(Statement *stmt);
    void addElseStmt(Statement *stmt);

private:
    int index;
    StatementType statementType;

    optional<Variable *> variable;
    optional<ProcName> procName;

    optional<vector<string>> expressionLst;
    set<Variable *> expressionVars;
    set<ConstantValue *> expressionConsts;

    optional<vector<Statement *>> thenStmtLst;
    optional<vector<Statement *>> elseStmtLst;
};