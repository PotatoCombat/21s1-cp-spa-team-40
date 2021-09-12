#pragma once
#include <string>
#include <vector>

#include "common/model/condition/Condition.h"
#include "common/model/statement/Statement.h"

using namespace std;

class WhileStatement : public Statement {
private:
    Condition *cond;
    vector<Statement *> stmtLst;
    vector<string> condLst;

public:
    // constructor
    WhileStatement(int index, Condition *cond);

    WhileStatement(int index);

    // adder
    void addStatement(Statement *statement);

    void setCondLst(vector<string> condLst);

    // getters
    Condition *getCondition() override;

    vector<Statement *> getThenStmtLst() override;
};
