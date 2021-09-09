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
    const char SEMICOLON = ';';

    // helper methods
    string trimString(string input);
    string trimLString(string input);
    string trimRString(string input);
    DeclTuple splitDecl(string input);
    RelTuple splitBCB(string input);

public:
    QueryTokenizer() {}

    pair<string, string> splitIntoParts(string queryString);

    tuple<vector<DeclTuple>, string, vector<RelTuple>, vector<PatTuple>> tokenize(string input);

    vector<DeclTuple> tokenizeDeclaration(string declaration);
    string tokenizeReturnEntity(string clause);
    vector<RelTuple> tokenizeSuchThatClause(string clause);
    vector<PatTuple> tokenizePatternClause(string clause);
};
