#pragma once

#include "common/model/ConstantValue.h"
#include "common/model/Variable.h"
#include "common/model/statement/Statement.h"
#include <string>
#include <vector>

using namespace std;

class IfStatement : public Statement {
private:
    vector<Statement> thenStmtLst;
    vector<Statement> elseStmtLst;
    vector<string> condLst;
    vector<ConstantValue> constLst;
    vector<Variable> varLst;

public:
    IfStatement(int index);

    void addThenStatement(Statement statement);
    void addElseStatement(Statement statement);

    void setExpressionLst(vector<string> condLst);

    void addExpressionVar(Variable var);

    void addExpressionConst(ConstantValue constVal);

    vector<Statement> getThenStmtLst();

    vector<Statement> getElseStmtLst();
};
