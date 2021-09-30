#pragma once

#include <string>

#include "pkb/PKB.h"

#include "query_processor/Result.h"
#include "query_processor/model/Clause.h"
#include "query_processor/model/PatternClause.h"
#include "query_processor/relationship_handler/ClauseHandlerError.h"
#include "query_processor/relationship_handler/ModifiesStmtHandler.h"
#include "query_processor/model/DesignEntityTypeHelper.h"

using namespace std;

class AssignPatternHandler {
private:
    PatternClause *patternClause;
    PKB *pkb;
    void validate();

public:
    AssignPatternHandler(PatternClause *patternClause, PKB *pkb)
        : patternClause(patternClause), pkb(pkb) {}
    Result eval();
};
