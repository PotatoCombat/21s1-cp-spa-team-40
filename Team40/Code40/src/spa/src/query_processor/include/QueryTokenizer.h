#pragma once

#include <string>
#include <vector>

using namespace std;

class QueryTokenizer {
private:
    // TODO: have the keywords "Select" etc stored as class constants
    const string KEYWORD_SELECT = "Select";
    const string KEYWORD_SUCH_THAT = "such that";
    const string KEYWORD_PATTERN = "pattern";
    const string WHITESPACE_SET = " \n\t\r";
    const char WHITESPACE = ' ';
    const char L_BRACKET = '(';
    const char COMMA = ',';
    const char R_BRACKET = ')';
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
