#pragma once

#include <string>

#include "Reference.h"

using namespace std;

//enum class PATTERN_MATCH {
//    EXACT_MATCH, SUB_MATCH
//};

class PatternClause {
private:
    Reference stmt;
    Reference var;
    string pattern;

public:
    PatternClause(Reference stmt, Reference var, string pattern);
    Reference *getStmt();
    Reference *getVar();
    string getPattern();
    bool equals(PatternClause& other);
    PatternClause* copy();
    ~PatternClause();
};
