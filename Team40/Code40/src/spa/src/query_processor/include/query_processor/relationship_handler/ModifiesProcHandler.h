#pragma once

#include <set>
#include <string>
#include <vector>

#include "ClauseHandler.h"
#include "ClauseHandlerError.h"

#include "pkb/PKB.h"

#include "query_processor/ValueToPointersMap.h"

#include "query_processor/Result.h"
#include "query_processor/model/Clause.h"
#include "query_processor/model/Reference.h"

using namespace std;

class ModifiesProcHandler : public ClauseHandler {
private:
    void validate();

public:
    ModifiesProcHandler(Clause *clause, PKB *pkb)
        : ClauseHandler(clause, pkb) {}
    Result eval(int ref1Index, int ref2Index);
};
