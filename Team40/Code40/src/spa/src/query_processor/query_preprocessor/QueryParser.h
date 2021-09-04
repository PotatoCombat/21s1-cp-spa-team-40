#pragma once

#include "pql_model/clause/PatternClause.cpp"
#include "pql_model/clause/SelectClause.cpp"
#include "pql_model/clause/SuchThatClause.cpp"

using namespace std;

class QueryParser {
private:
    vector<string> tokenizeClause(string clause);
    vector<Declaration> parseDeclarations(vector<string> declarations);
    vector<SuchThatClause> parseSuchThatClause(vector<string> clause);
    vector<PatternClause> parsePatternClause(vector<string> clause);

public:
    QueryParser();
    SelectClause parseQuery(vector<string> declarations,
               tuple<string, vector<string>, vector<string>> clause);
};
