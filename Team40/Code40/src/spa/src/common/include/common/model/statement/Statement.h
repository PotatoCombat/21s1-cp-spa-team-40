#pragma once

#include "common/model/ConstantValue.h"
#include "common/model/Procedure.h"
#include "common/model/Program.h"
#include "common/model/Variable.h"
#include <optional>
#include <stdexcept>
#include <vector>

enum class StatementType { UNKNOWN, READ, PRINT, ASSIGN, CALL, WHILE, IF };

class Statement {
public:
    Statement(int index, StatementType statementType);

    int getIndex();
    StatementType getStatementType();

    Variable getVariable();
    Procedure getProcedure();

    vector<string> getExpressionLst();
    vector<Variable> getExpressionVars();
    vector<ConstantValue> getExpressionConsts();

    vector<Statement> getThenStmtLst();
    vector<Statement> getElseStmtLst();

    void setVariable(Variable variable);
    void setProcedure(Procedure procedure);

    void setExpressionLst(vector<string> expressionLst);
    void addExpressionVar(Variable expressionVar);
    void addExpressionConst(ConstantValue expressionConst);

    void addThenStmt(Statement stmt);
    void addElseStmt(Statement stmt);

private:
    int index;
    StatementType statementType;

    optional<Variable> variable;
    optional<Procedure> procedure;

    optional<vector<string>> expressionLst;
    optional<vector<Variable>> expressionVars;
    optional<vector<ConstantValue>> expressionConsts;

    optional<vector<Statement>> thenStmtLst;
    optional<vector<Statement>> elseStmtLst;
};