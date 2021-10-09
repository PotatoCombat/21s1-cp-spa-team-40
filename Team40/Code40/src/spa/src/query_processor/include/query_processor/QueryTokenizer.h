#pragma once

#include <algorithm>
#include <cctype>
#include <string>
#include <tuple>
#include <vector>

#include "query_processor/Abstractions.h"

#include "query_processor/exception/SyntaxError.h"
#include "query_processor/model/DesignEntityTypeHelper.h"

using namespace std;

class QueryTokenizer {
public:
    QueryTokenizer() = default;

    pair<string, string> separateQueryString(string input);
    void tokenizeDeclarations(string input, vector<DeclPair> &decls);
    vector<string> tokenizeReturnSynonyms(string input, string &remaining);
    void tokenizeClauses(string input, vector<ClsTuple> &suchThatClauses,
                         vector<PatTuple> &patternClauses,
                         vector<WithTuple> &withClauses);
    vector<PatToken> tokenizePattern(vector<string> patArgs);

private:
    // "get" methods are expectant - if the thing you are trying to get
    // doesn't exist, it throws an exception (in contrast to "find" methods)
    vector<string> tokenizeReturnTuple(string input, string &remaining);
    string tokenizeReturnRef(string input, string &remaining);
    size_t tokenizeSuchThat(string input, size_t startPos, ClsTuple &clause);
    size_t tokenizePattern(string input, size_t startPos, PatTuple &clause);
    size_t tokenizeWith(string input, size_t startPos, WithTuple &clause);

    bool isQuotedString(string input);
    bool isWildcard(string input);
    bool hasNoWhitespace(string input);
    string trim(string input);
    string trimL(string input);
    string trimR(string input);
    size_t findNextWhitespace(string input, size_t pos);
    size_t findNextToken(string input, size_t pos);
    size_t findPatternDelimiter(string input, size_t pos);
    string getTokenBeforeX(string input, char x, size_t startPos,
                           size_t &nextPos);
    size_t getPosAfterRBracket(string input, size_t startPos);
    string removeWhitespaceWithinQuotes(string input);
    string removeWhitespaceAroundPeriod(string input);
    vector<string> tokenizeCommaSeparatedValues(string input);
    string extractPatternString(string input);

    DesignEntityTypeHelper deHelper = DesignEntityTypeHelper();
    // ClauseTypeHelper clsHelper = ClauseTypeHelper();

    inline static const string KEYWORD_SELECT = "Select";
    inline static const string KEYWORD_SUCH = "such";
    inline static const string KEYWORD_THAT = "that";
    inline static const string KEYWORD_PATTERN = "pattern";
    inline static const string KEYWORD_AND = "and";
    inline static const string KEYWORD_WITH = "with";
    inline static const string KEYWORD_BOOLEAN = "BOOLEAN";
    inline static const string WHITESPACE_SET = " \n\t\r";
    inline static const string PATTERN_DELIMITER_SET = "()+-*/%_\"";

    inline static const char SEMICOLON = ';';
    inline static const char COMMA = ',';
    inline static const char L_BRACKET = '(';
    inline static const char R_BRACKET = ')';
    inline static const char QUOTE = '"';
    inline static const char UNDERSCORE = '_';
    inline static const char EQUAL = '=';
    inline static const char PERIOD = '.';
    inline static const char L_CARROT = '<';
    inline static const char R_CARROT = '>';
};
