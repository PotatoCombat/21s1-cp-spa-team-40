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

class UsesProcHandler : public ClauseHandler {
public:
    UsesProcHandler(Clause *clause, PKB *pkb)
        : ClauseHandler(clause, pkb) {}
    Result eval();
};
