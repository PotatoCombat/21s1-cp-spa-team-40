#pragma once

#include <algorithm>
#include <vector>

#include "Abstractions.h"

#include "query_processor/model/Clause.h"
#include "query_processor/model/ClauseTypeHelper.h"
#include "query_processor/model/DesignEntityTypeHelper.h"
#include "query_processor/model/PatternClause.h"
#include "query_processor/model/Reference.h"
#include "query_processor/exception/ValidityError.h"

using namespace std;

class QueryParser {
private:
    DesignEntityTypeHelper deHelper;
    ClauseTypeHelper clsHelper;

    ReferenceType checkRefType(string &val);
    bool isInteger(string val);
    bool isNamedSynonym(string &val);
    bool isWildcard(string val);

public:
    QueryParser() = default;
    Reference *parseDeclaration(DeclPair declaration);
    Clause *parseClause(ClsTuple clause, vector<Reference *> &declList);
    PatternClause *parsePattern(PatTuple pattern, vector<Reference*>& declList);
};
