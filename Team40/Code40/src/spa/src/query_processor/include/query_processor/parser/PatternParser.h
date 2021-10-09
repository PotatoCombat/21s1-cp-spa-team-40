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
    bool isAssignPattern(Reference *identity);
    bool isWhilePattern(Reference *identity);
    bool isIfPattern(Reference *identity);
    bool isExactPattern(string pattern);

    vector<Reference *> declList;

    string ident;
    vector<string> args;
};
