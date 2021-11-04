#pragma once

#include <optional>
#include <set>
#include <stdexcept>
#include <vector>

#include "common/model/Abstractions.h"
#include "common/model/ConstantValue.h"
#include "common/model/Variable.h"

enum class StatementType {
    UNKNOWN,
    READ,
    PRINT,
    ASSIGN,
    CALL,
    WHILE,
    IF,
    TERMINAL
};

class Statement : public Entity<StmtIndex> {
public:
    Statement();
    explicit Statement(StmtIndex index, StatementType statementType);

    StatementType getStatementType();

    Variable *getVariable();
    ProcName getProcName();

    vector<string> getExpressionLst();
    set<Variable *, Comparator<Variable>> getExpressionVars();
    set<ConstantValue *, Comparator<ConstantValue>> getExpressionConsts();

    vector<Statement *> getThenStmtLst();
    vector<Statement *> getElseStmtLst();

    void setVariable(Variable *variable);
    void setProcName(ProcName procName);

    void setExpressionLst(vector<string> expressionLst);
    void addExpressionVar(Variable *expressionVar);
    void addExpressionConst(ConstantValue *expressionConst);

    void addThenStmt(Statement *stmt);
    void addElseStmt(Statement *stmt);

    bool operator==(const Statement &other) const;

private:
    StatementType statementType;

    optional<Variable *> variable;
    optional<ProcName> procName;

    optional<vector<string>> expressionLst;
    set<Variable *, Comparator<Variable>> expressionVars;
    set<ConstantValue *, Comparator<ConstantValue>> expressionConsts;

    optional<vector<Statement *>> thenStmtLst;
    optional<vector<Statement *>> elseStmtLst;
};
