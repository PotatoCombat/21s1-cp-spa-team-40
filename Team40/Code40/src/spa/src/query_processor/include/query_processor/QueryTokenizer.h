#pragma once

#include <algorithm>
#include <string>

#include "query_processor/Abstractions.h"
#include "query_processor/exception/SyntaxError.h"
#include "query_processor/model/DesignEntityTypeHelper.h"

using namespace std;

class QueryTokenizer {
public:
    QueryTokenizer() = default;

    pair<string, string> separateQueryString(string input);
    void tokenizeDeclarations(string input, vector<DeclPair> &decls);
    void tokenizeReturnSynonym(string input, string &returnSynonym,
                               string &remaining);
    void tokenizeClauses(string input, vector<ClsTuple> &suchThatClauses,
                         vector<PatTuple> &patternClauses,
                         vector<WithTuple> &withClauses);

private:
    vector<string> tokenizeDeclarationSynonym(string input);
    size_t tokenizeSuchThat(string input, size_t startPos, ClsTuple &clause);
    size_t tokenizePattern(string input, size_t startPos, PatTuple &clause);
    size_t tokenizeWith(string input, size_t startPos, WithTuple &clause);

    string trim(string input);
    string trimL(string input);
    string trimR(string input);
    size_t findNextWhitespace(string input, size_t pos);
    size_t findNextToken(string input, size_t pos);
    string parseValidName(string input);
    string getTokenBeforeX(string input, char x, size_t startPos,
                           size_t &nextPos);
    string removeWhitespaceWithinQuotes(string input);
    string extractPatternString(string input);

    DesignEntityTypeHelper deHelper = DesignEntityTypeHelper();
    // ClauseTypeHelper clsHelper = ClauseTypeHelper();

    enum CLAUSE_CODES {
        SUCH_THAT_CLAUSE,
        PATTERN_CLAUSE,
        WITH_CLAUSE,
        NULL_CLAUSE
    };

    enum STATE_CODES {
        READ_TYPE_STATE,
        SUCH_INTERMEDIATE_STATE,
        READ_ARGS_STATE,
        INVALID_STATE
    };

    inline static const string KEYWORD_SELECT = "Select";
    inline static const string KEYWORD_SUCH = "such";
    inline static const string KEYWORD_THAT = "that";
    inline static const string KEYWORD_PATTERN = "pattern";
    inline static const string KEYWORD_AND = "and";
    inline static const string KEYWORD_WITH = "with";
    inline static const string WHITESPACE_SET = " \n\t\r";

    inline static const char SEMICOLON = ';';
    inline static const char COMMA = ',';
    inline static const char L_BRACKET = '(';
    inline static const char R_BRACKET = ')';
    inline static const char QUOTE = '"';
    inline static const char UNDERSCORE = '_';
    inline static const char EQUAL = '=';
    inline static const char PERIOD = '.';
};
