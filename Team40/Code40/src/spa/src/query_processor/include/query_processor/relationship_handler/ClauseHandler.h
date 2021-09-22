#pragma once

#include "ClauseHandlerError.h"

#include "common/model/Statement.h"

#include "pkb/PKB.h"

#include "query_processor/Result.h"
#include "query_processor/model/Clause.h"

class ClauseHandler {
protected:
    Clause *clause;
    PKB *pkb;
    ClauseHandler(Clause *clause, PKB *pkb) : clause(clause), pkb(pkb) {}

public:
    // evaluates the clause and writes the answer to the result object
    // Result invalid = false only happens when neither of the references is
    // synonym
    virtual Result eval(int ref1Index, int ref2Index) = 0;
    static StatementType desTypeToStmtType(DesignEntityType desType);
    static bool isDesTypeStmtType(DesignEntityType desType,
                                  StatementType stmtType);
};
