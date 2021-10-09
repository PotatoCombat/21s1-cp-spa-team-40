#pragma once

#include <string>
#include <vector>

#include "query_processor/Abstractions.h"

#include "query_processor/exception/ValidityError.h"
#include "query_processor/model/Clause.h"
#include "query_processor/model/Reference.h"
#include "query_processor/parser/ParserUtil.h"

class PatternParser {
public:
    PatternParser();

    void initReferences(vector<Reference *> &declList);
    void clear();
    Clause *parse(PatTuple patTuple);

private:
    Clause *parseAssign(Reference *identity);
    Clause *parseWhile(Reference *identity);
    Clause *parseIf(Reference *identity);
    Reference *parseValidVariable(string var);

    Reference *getReferenceIfDeclared(string syn);
    bool isAssignPatternClause(Reference *identity);
    bool isWhilePatternClause(Reference *identity);
    bool isIfPatternClause(Reference *identity);
    bool isExactPattern(vector<PatToken> pattern);
    bool isWildcardPattern(vector<PatToken> pattern);
    bool isQuotedPattern(vector<PatToken> pattern);
    bool isUnderscoreQuotedPattern(vector<PatToken> pattern);

    vector<Reference *> declList;

    string ident;
    string var;
    vector<string> tokens;
};
