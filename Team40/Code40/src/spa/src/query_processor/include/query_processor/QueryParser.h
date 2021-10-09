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
    Reference *parseReturnSynonym(string ref);
    Clause *parseSuchThatClause(ClsTuple clsTuple);
    Clause *parsePatternClause(PatTuple patTuple);

    void clear();

private:
    bool isValidAttribute(DesignEntityType type, string attribute);
    ReferenceAttribute parseValidAttr(string ref);
    string parseValidName(string input);
    void clearDeclarations();

    DesignEntityTypeHelper deHelper;
    SuchThatParser stParser;
    PatternParser ptParser;

    vector<Reference *> declList;
};
