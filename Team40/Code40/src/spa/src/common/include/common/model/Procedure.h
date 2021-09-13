#pragma once
#include "common/model/statement/Statement.h"
#include <string>
#include <vector>
using namespace std;

class Procedure {
private:
    string name;
    vector<Statement *> stmtLst;

public:
    Procedure(string name);

    void addToStmtLst(Statement *stmt);

    string getName();

    vector<Statement *> getStmtLst();
};
