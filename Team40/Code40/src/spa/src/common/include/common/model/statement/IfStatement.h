#pragma once

#include "common/model/ConstantValue.h"
#include "common/model/Variable.h"
#include "common/model/statement/Statement.h"
#include <string>
#include <vector>

using namespace std;

class IfStatement : public Statement {
private:
    vector<Statement *> thenStmtLst;
    vector<Statement *> elseStmtLst;
    vector<ConstantValue> constLst;
    vector<string> condLst;
    vector<Variable> varLst;

public:
    IfStatement(int index);

    void addThenStatement(Statement *statement);
    void addElseStatement(Statement *statement);

    void setCondLst(vector<string> condLst);

    void addVar(Variable var);

    void addConst(ConstantValue constVal);

    vector<Statement *> getThenStmtLst() override;

    vector<Statement *> getElseStmtLst() override;
};
