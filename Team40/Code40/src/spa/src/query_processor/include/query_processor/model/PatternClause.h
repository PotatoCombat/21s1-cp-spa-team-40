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
<<<<<<< HEAD:Team40/Code40/src/spa/src/query_processor/include/query_processor/model/PatternClause.h
    PatternClause(Reference stmt, Reference var, string pattern);
    Reference getStmt();
    Reference getVar();
=======
    Pattern(Reference stmt, Reference var, string pattern);
    Reference *getStmt();
    Reference *getVar();
>>>>>>> master:Team40/Code40/src/spa/src/query_processor/include/query_processor/model/Pattern.h
    string getPattern();
};
