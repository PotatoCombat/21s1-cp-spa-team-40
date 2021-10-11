#pragma once

#include <string>
#include <set>
#include <vector>

#include "pkb/PKB.h"

#include "query_processor/Result.h"
#include "query_processor/model/Clause.h"
#include "query_processor/relationship_handler/ClauseHandlerError.h"
#include "query_processor/relationship_handler/ModifiesStmtHandler.h"
#include "query_processor/model/DesignEntityTypeHelper.h"

using namespace std;

class PatternHandler {
private:
    Clause *patternClause;
    PKB *pkb;
    void validate();

public:
    PatternHandler(Clause *patternClause, PKB *pkb)
        : patternClause(patternClause), pkb(pkb) {}
    Result eval();
};
