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

    Clause *parseAssign(Reference *identity);
    Clause *parseWhile(Reference *identity);
    Clause *parseIf(Reference *identity);

    bool isAssignPatternClause(Reference *identity);
    bool isWhilePatternClause(Reference *identity);
    bool isIfPatternClause(Reference *identity);
    bool isExactPattern(vector<PatToken> pattern);
    bool isWildcardPattern(vector<PatToken> pattern);
    bool isQuotedPattern(vector<PatToken> pattern);
    bool isUnderscoreQuotedPattern(vector<PatToken> pattern);
    bool isOperator(string token);
    bool isLBracket(string token);
    bool isRBracket(string token);
    int countOccurences(vector<string> vec, string token);

    string ref1;
    string ref2;
    vector<string> tokens;
};
