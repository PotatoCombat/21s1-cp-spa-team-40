#pragma once

#include <string>
#include <vector>

using namespace std;

class QueryTokenizer {
private:
    // TODO: have the keywords "Select" etc stored as class constants
    string trimString(string input);
    pair<string, string> splitDecl(string input);
    tuple<string, string, string> splitBCB(string input);

public:
    QueryTokenizer() {}

    pair<string, string> splitIntoParts(string queryString);

    vector<pair<string, string>> tokenizeDeclaration(string declaration);
    string tokenizeReturnEntity(string clause);
    vector<tuple<string, string, string>> tokenizeSuchThatClause(string clause);
    vector<tuple<string, string, string>> tokenizePatternClause(string clause);
};
