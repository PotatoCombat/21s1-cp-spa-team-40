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

    DesignEntityTypeHelper deHelper;
    RelationTypeHelper refHelper;

    // helpers
    static string trim(string input);
    static string trimL(string input);
    static string trimR(string input);
    static void splitComma(string input, vector<string> &vec);
    static size_t findFirstWhitespace(string input);
    template<typename T> static void splitBCB(string input, T& vec);

public:
    QueryTokenizer() = default;

    pair<string, string> separateDeclaration(string input);
    string tokenizeReturn(string input, string &remaining);
    void tokenizeDeclaration(string input, vector<DeclPair> &decls);
    void tokenizeClause(string input, vector<RelTuple> &rels, vector<PatTuple> &pats);
};
