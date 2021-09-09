#pragma once

#include "clause/PatternClause.h"
#include "clause/SelectClause.h"
#include "clause/SuchThatClause.h"

using namespace std;

class QueryParser {
public:
    QueryParser();
    Declaration parseDeclaration(pair<string, string> declaration);
    SuchThatClause parseSuchThatClause(tuple<string, string, string> clause);
    //PatternClause parsePatternClause(tuple<string, string, string> clause);
};
