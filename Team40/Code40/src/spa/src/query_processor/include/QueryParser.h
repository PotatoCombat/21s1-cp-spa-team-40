#pragma once

#include "Reference.h"
#include "Clause.h"

using namespace std;

class QueryParser {
public:
    QueryParser();
    Reference parseDeclaration(pair<string, string> declaration);
    Clause parseSuchThatClause(tuple<string, string, string> clause);
    //PatternClause parsePatternClause(tuple<string, string, string> clause);
};
