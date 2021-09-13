#pragma once
#include <string>
#include <vector>
#include "common/model/Statement.h"
using namespace std;

class Procedure {
private:
    string name; // passing 'const string' {aka 'const
                 // std::__cxx11::basic_string<char>'} as 'this' argument
                 // discards qualifiers [-fpermissive]gcc
    vector<Statement> stmtLst;

public:
    Procedure(string name);

    void addToStmtLst(Statement stmt);

    string getName();

    vector<Statement> getStmtLst();
};
