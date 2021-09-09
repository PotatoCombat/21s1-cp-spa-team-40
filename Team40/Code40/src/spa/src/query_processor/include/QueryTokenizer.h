#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "Abstractions.h"

using namespace std;

class QueryTokenizer {
private:
    const string KEYWORD_SELECT = "Select";
    const string KEYWORD_SUCH_THAT = "such that";
    const string KEYWORD_PATTERN = "pattern";
    const string WHITESPACE_SET = " \n\t\r";
    const char WHITESPACE = ' ';
    const char L_BRACKET = '(';
    const char COMMA = ',';
    const char R_BRACKET = ')';

    // helper methods
    string trimString(string input);
    DeclTuple splitDecl(string input);
    RelTuple splitBCB(string input);

public:
    QueryTokenizer() {}

    pair<string, string> splitIntoParts(string queryString);

    vector<DeclTuple> tokenizeDeclaration(string declaration);
    string tokenizeReturnEntity(string clause);
    vector<RelTuple> tokenizeSuchThatClause(string clause);
    vector<PatTuple> tokenizePatternClause(string clause);
};
