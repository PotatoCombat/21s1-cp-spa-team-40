#pragma once

#include <string>

#include "Reference.h"

using namespace std;

class PatternClause {
private:
    Reference stmt;
    Reference var;
    string pattern;

public:
    PatternClause(Reference stmt, Reference var, string pattern);
    Reference getStmt();
    Reference getVar();
    string getPattern();
};
