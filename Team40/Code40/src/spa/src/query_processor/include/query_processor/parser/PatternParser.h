#pragma once

#include <string>
#include <vector>

#include "query_processor/parser/ClauseParser.h"

class PatternParser : public ClauseParser {
public:
    PatternParser() : ClauseParser() {}

    void clear();

    vector<PatToken> parsePatternTokens(vector<PatToken> tokens);

private:
    Clause *parsePt(PatTuple patTuple) override;

    Clause *parseAssign();
    Clause *parseWhile();
    Clause *parseIf();

    bool isAssignPatternClause();
    bool isWhilePatternClause();
    bool isIfPatternClause();
    bool isExactPattern(vector<PatToken> pattern);
    bool isWildcardPattern(vector<PatToken> pattern);
    bool isQuotedPattern(vector<PatToken> pattern);
    bool isUnderscoreQuotedPattern(vector<PatToken> pattern);
    bool isOperator(string token);
    bool isLBracket(string token);
    bool isRBracket(string token);
    int countOccurences(vector<string> vec, string token);

    Reference *r1 = nullptr;
    Reference *r2 = nullptr;
    string ref1;
    string ref2;
    vector<string> tokens;
};
