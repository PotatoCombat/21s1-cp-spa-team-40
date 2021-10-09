#pragma once

#include <string>
#include <vector>

#include "query_processor/Abstractions.h"

#include "query_processor/exception/ValidityError.h"
#include "query_processor/model/PatternClause.h"
#include "query_processor/model/Reference.h"
#include "query_processor/parser/ParserUtil.h"

class PatternParser {
public:
    PatternParser();

    void initReferences(vector<Reference *> &declList);
    void clear();
    PatternClause *parse(PatTuple patTuple);

private:
    PatternClause *parseAssign(Reference *identity);
    PatternClause *parseWhile(Reference *identity);
    PatternClause *parseIf(Reference *identity);
    Reference *parseValidVariable(string var);

    Reference *getReferenceIfDeclared(string syn);
    bool isAssignPattern(Reference *identity);
    bool isWhilePattern(Reference *identity);
    bool isIfPattern(Reference *identity);
    bool isExactPattern(string pattern);

    vector<Reference *> declList;

    string ident;
    vector<string> args;
};
