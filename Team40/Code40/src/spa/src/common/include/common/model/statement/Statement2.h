#pragma once

#include "common/model/ConstantValue.h"
#include "common/model/Procedure.h"
#include "common/model/Program.h"
#include "common/model/Variable.h"
#include <optional>
#include <stdexcept>
#include <vector>

//enum class StatementType { UNKNOWN, READ, PRINT, ASSIGN, CALL, WHILE, IF };

class Statement2 {
public:
    Statement2(int index, StatementType statementType);

    int getIndex();
    StatementType getStatementType();

    Variable getVariable();
    Procedure getProcedure();

    vector<string> getExpressionLst();
    vector<Variable> getExpressionVars();
    vector<ConstantValue> getExpressionConsts();

    vector<Statement2> getThenStmtLst();
    vector<Statement2> getElseStmtLst();

    void setVariable(Variable variable);
    void setProcedure(Procedure procedure);

    void setExpressionLst(vector<string> expressionLst);
    void addExpressionVar(Variable expressionVar);
    void addExpressionConst(ConstantValue expressionConst);

    void addThenStmt(Statement2 stmt);
    void addElseStmt(Statement2 stmt);

private:
    int index;
    StatementType statementType;

    optional<Variable> variable;
    optional<Procedure> procedure;

    optional<vector<string>> expressionLst;
    optional<vector<Variable>> expressionVars;
    optional<vector<ConstantValue>> expressionConsts;

    optional<vector<Statement2>> thenStmtLst;
    optional<vector<Statement2>> elseStmtLst;
};
