#pragma once

#include <string>
#include <vector>

#include "query_processor/model/Clause.h"
#include "query_processor/model/DesignEntityTypeHelper.h"
#include "query_processor/model/Reference.h"
#include "query_processor/parser/ParserUtil.h"
#include "query_processor/parser/PatternParser.h"
#include "query_processor/parser/SuchThatParser.h"

using namespace std;

class QueryParser {
public:
    QueryParser() = default;

    void parseDeclarations(vector<DeclPair> declPairs);
    Reference *parseReturnSynonyms(string ref);
    Clause *parseSuchThatClause(ClsTuple clsTuple);
    Clause *parsePatternClause(PatTuple patTuple);

    void clear();

private:
    ReferenceAttribute parseValidAttr(string ref);
    void clearDeclarations();

    DesignEntityTypeHelper deHelper;
    SuchThatParser stParser;
    PatternParser ptParser;

    vector<Reference *> declList;
};
