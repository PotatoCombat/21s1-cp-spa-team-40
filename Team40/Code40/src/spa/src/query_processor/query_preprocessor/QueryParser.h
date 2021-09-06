#pragma once

#include "pql_model/clause/PatternClause.h"
#include "pql_model/clause/SelectClause.h"
#include "pql_model/clause/SuchThatClause.h"

using namespace std;

class QueryParser {
public:
    QueryParser();
    Declaration parseDeclaration(pair<string, string> declaration);
    SuchThatClause parseSuchThatClause(tuple<string, string, string> clause);
    //PatternClause parsePatternClause(tuple<string, string, string> clause);
};
