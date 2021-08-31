#pragma once
#include <string>
#include <vector>
#include "Statement.h"
using namespace std;

class Procedure {
private:
    const string name;
    vector<Statement> stmtLst;

public:
    Procedure(string name);

    void addStatement(Statement statement);

    string getName();

    vector<Statement> getStmtLst();
};
