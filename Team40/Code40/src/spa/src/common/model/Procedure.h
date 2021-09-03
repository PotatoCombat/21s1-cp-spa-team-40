#pragma once
#include "statement/Statement.h"
#include <string>
#include <vector>
using namespace std;

class Procedure {
private:
    const string name;
    vector<Statement> stmtLst;

public:
    Procedure(string name);

    void addToStmtLst(Statement stmt);

    string getName();

    vector<Statement> getStmtLst();
};
