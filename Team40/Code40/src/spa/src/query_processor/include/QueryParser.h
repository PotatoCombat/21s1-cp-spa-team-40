#pragma once

#include "Reference.h"
#include "Relation.h"

using namespace std;

class QueryParser {
public:
    QueryParser();
    Reference parseDeclaration(pair<string, string> declaration);
    Relation parseSuchThatClause(tuple<string, string, string> clause);
    //PatternClause parsePatternClause(tuple<string, string, string> clause);
};
