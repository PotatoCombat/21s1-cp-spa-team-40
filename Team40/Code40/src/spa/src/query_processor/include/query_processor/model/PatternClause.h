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
    bool exactMatch;

public:
    PatternClause(Reference stmt, Reference var, string pattern, bool isExactMatch);
    PatternClause(Reference stmt, Reference var);
    Reference *getStmt();
    Reference *getVar();
    string getPattern();
    bool isExactMatch();
    bool equals(PatternClause& other);
    PatternClause* copy();
    ~PatternClause();
};
