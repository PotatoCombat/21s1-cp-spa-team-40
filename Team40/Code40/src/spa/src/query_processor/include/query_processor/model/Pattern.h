#pragma once

#include <string>

#include "Reference.h"

using namespace std;

class Pattern {
private:
    Reference stmt;
    Reference var;
    string pattern;
public:
    Pattern(Reference stmt, Reference var, string pattern);
    Reference *getStmt();
    Reference *getVar();
    string getPattern();
};
