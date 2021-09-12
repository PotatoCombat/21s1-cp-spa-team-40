#pragma once

#include <string>
#include <vector>

#include "ClauseHandler.h"
#include "ClauseHandlerError.h"

#include "pkb/PKB.h"

#include "query_processor/Result.h"
#include "query_processor/model/Clause.h"
#include "query_processor/model/Reference.h"

using namespace std;

class UsesProcHandler : public ClauseHandler {
private:
    void validate();

public:
    UsesProcHandler(Clause *clause, PKB *pkb) : ClauseHandler(clause, pkb) {}
    Result eval();
};
