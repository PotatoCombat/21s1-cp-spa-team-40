#pragma once

#include "pql_model/clause/PatternClause.cpp"
#include "pql_model/clause/SelectClause.cpp"
#include "pql_model/clause/SuchThatClause.cpp"

using namespace std;

class QueryParser {
public:
    QueryParser();
    Declaration parseDeclaration(pair<string, string> declaration);
    SuchThatClause parseSuchThatClause(tuple<string, string, string> clause);
    PatternClause parsePatternClause(tuple<string, string, string> clause);
};
