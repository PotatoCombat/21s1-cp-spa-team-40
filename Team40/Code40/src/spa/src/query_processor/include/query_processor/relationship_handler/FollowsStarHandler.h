#pragma once

#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "ClauseHandler.h"
#include "ClauseHandlerError.h"

#include "pkb/PKB.h"

#include "query_processor/Result.h"
#include "query_processor/model/Clause.h"
#include "query_processor/model/Reference.h"

using namespace std;

class FollowsStarHandler : public ClauseHandler {
private:
    void validate();

public:
    FollowsStarHandler(Clause *clause, PKB *pkb)
        : ClauseHandler(clause, pkb) {}
    Result eval();
};
