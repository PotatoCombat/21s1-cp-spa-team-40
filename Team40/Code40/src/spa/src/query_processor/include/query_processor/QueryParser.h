#pragma once

#include <string>
#include <vector>

#include "query_processor/model/Clause.h"
#include "query_processor/model/DesignEntityTypeHelper.h"
#include "query_processor/model/PatternClause.h"
#include "query_processor/model/Reference.h"
#include "query_processor/parser/PatternParser.h"
#include "query_processor/parser/SuchThatParser.h"

using namespace std;

class QueryParser {
public:
    QueryParser() = default;

    void parseDeclarations(vector<DeclPair> declPairs);
    Reference *parseReturnSynonym(string ref);
    Clause *parseSuchThatClause(ClsTuple clsTuple);
    PatternClause *parsePatternClause(PatTuple patTuple);

    void clear();

private:
    bool isValidAttribute(DesignEntityType type, string attribute);
    ReferenceAttribute getAttrRef(string ref);
    void deleteDeclarations();

    DesignEntityTypeHelper deHelper;
    SuchThatParser stParser;
    PatternParser ptParser;

    vector<Reference *> declList;
};
