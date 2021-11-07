#pragma once

#include <string>
#include <vector>

#include "query_processor/parser/ClauseParser.h"

class PatternParser : public ClauseParser {
public:
    PatternParser() : ClauseParser() {}

    void clear();

    // meant to be private, public just for testing purposes
    vector<PatToken> parsePatternTokens();

private:
    Clause *parsePt(PatTuple patTuple) override;

    Clause *parseAssign();

    bool isAssignPatternClause();
    bool isWhilePatternClause();
    bool isIfPatternClause();
    bool isExactPattern(vector<PatToken> pattern);
    bool isWildcardPattern(vector<PatToken> pattern);
    bool isQuotedPattern(vector<PatToken> pattern);
    bool isUnderscoreQuotedPattern(vector<PatToken> pattern);

    Reference *r1 = nullptr;
    Reference *r2 = nullptr;
    string ref1;
    string ref2;
    vector<string> tokens;

    inline static const int PATARG_SIZE_ONE = 1;
    inline static const int PATARG_SIZE_TWO = 2;
};
