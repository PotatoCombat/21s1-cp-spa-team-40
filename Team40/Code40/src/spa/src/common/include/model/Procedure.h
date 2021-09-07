#pragma once
#include "model/statement/Statement.h"
#include <string>
#include <vector>
using namespace std;

class Procedure {
private:
    const string name;
    vector<Statement> stmtLst;

public:
    Procedure(string name, vector<Statement> stmtLst);

    string getName();

    vector<Statement> getStmtLst();
};
