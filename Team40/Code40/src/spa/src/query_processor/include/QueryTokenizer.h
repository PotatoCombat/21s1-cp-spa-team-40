#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "Abstractions.h"
#include "DesignEntityTypeHelper.h"
#include "RelationTypeHelper.h"

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

    // helpers
    DesignEntityTypeHelper deHelper;
    RelationTypeHelper refHelper;
    string trim(string input);
    string trimL(string input);
    string trimR(string input);
    void splitComma(string input, vector<string> &vec);
    size_t findFirstWhitespace(string input);
    void splitBCBRel(string input, RelTuple tup);

public:
    QueryTokenizer() = default;

    pair<string, string> separateDeclaration(string input);
    string tokenizeReturn(string input, string &remaining);
    void tokenizeDeclaration(string input, vector<DeclPair> &decls);
    void tokenizeClause(string input, vector<RelTuple> &rels, vector<PatTuple> &pats);
};
