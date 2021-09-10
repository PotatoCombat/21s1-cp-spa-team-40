#pragma once

#include <string>
#include <vector>

#include "Reference.h"
#include "Clause.h"
#include "ClauseHandler.h"
#include "ClauseHandlerError.h"
#include "Result.h"
#include "pkb/PKB.h"

using namespace std;

class ModifiesStmtHandler : public ClauseHandler {
public:
    ModifiesStmtHandler(Clause *clause, PKB *pkb)
        : ClauseHandler(clause, pkb) {}
    Result eval();
};
