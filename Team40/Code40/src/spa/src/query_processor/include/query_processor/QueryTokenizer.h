#pragma once

#include <algorithm>
#include <cctype>
#include <string>
#include <tuple>
#include <vector>

#include "query_processor/Abstractions.h"

#include "query_processor/exception/SyntaxError.h"
#include "query_processor/model/ClauseTypeHelper.h"
#include "query_processor/model/DesignEntityTypeHelper.h"

using namespace std;

class QueryTokenizer {
private:
    inline static const string KEYWORD_SELECT = "Select";
    inline static const string KEYWORD_SUCH = "such";
    inline static const string KEYWORD_THAT = "that";
    inline static const string KEYWORD_PATTERN = "pattern";
    inline static const string KEYWORD_AND = "and";
    inline static const string KEYWORD_WITH = "with";

    // inline static const string NAME_SET =
    //    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    inline static const string WHITESPACE_SET = " \n\t\r";
    inline static const char WHITESPACE = ' ';
    inline static const char L_BRACKET = '(';
    inline static const char COMMA = ',';
    inline static const char R_BRACKET = ')';
    inline static const char SEMICOLON = ';';

    // helpers
    DesignEntityTypeHelper deHelper = DesignEntityTypeHelper();
    ClauseTypeHelper clsHelper = ClauseTypeHelper();
    string trim(string input);
    string trimL(string input);
    string trimR(string input);
    void splitComma(string input, vector<string> &vec);
    size_t findFirstWhitespace(string input);
    string extractPatternString(string input);
    bool isValidName(string name);

public:
    QueryTokenizer() = default;

    pair<string, string> separateDeclaration(string input);
    string tokenizeReturn(string input, string &remaining);
    void tokenizeDeclaration(string input, vector<DeclPair> &decls);
    void tokenizeClause(string input, vector<ClsTuple> &clss,
                        vector<PatTuple> &pats);
};
