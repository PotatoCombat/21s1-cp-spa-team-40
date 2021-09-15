#pragma once

#include <vector>
#include <string>

#include "Abstractions.h"

#include "query_processor/exception/ValidityError.h"
#include "query_processor/model/Clause.h"
#include "query_processor/model/ClauseTypeHelper.h"
#include "query_processor/model/DesignEntityTypeHelper.h"
#include "query_processor/model/PatternClause.h"
#include "query_processor/model/Reference.h"
#include "query_processor/parser/StmtStmtParser.h"
#include "query_processor/parser/XEntParser.h"
#include "query_processor/parser/ParserUtil.h"

using namespace std;

class QueryParser {
private:
    DesignEntityTypeHelper deHelper;
    ClauseTypeHelper clsHelper;

public:
    QueryParser() = default;

    Reference *parseDeclaration(DeclPair declaration);
    Clause *parseClause(ClsTuple clause, vector<Reference *> &declList);
    PatternClause *parsePattern(PatTuple pattern,
                                vector<Reference *> &declList);
};
