#pragma once

#include "common/model/ConstantValue.h"
#include "common/model/Variable.h"
#include "common/model/statement/Statement.h"
#include <string>
#include <vector>

using namespace std;

class WhileStatement : public Statement {
private:
    vector<Statement> stmtLst;
    vector<ConstantValue> constLst;
    vector<string> condLst;
    vector<Variable> varLst;

public:
    WhileStatement(int index);

    void addStatement(Statement statement);

    void setExpressionLst(vector<string> condLst);

    void addExpressionVar(Variable var);

    void addExpressionConst(ConstantValue constVal);

    vector<Statement> getThenStmtLst();
};
