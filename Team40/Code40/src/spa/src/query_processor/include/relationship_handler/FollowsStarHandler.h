#pragma once

#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "Reference.h"
#include "Clause.h"
#include "ClauseHandler.h"
#include "ClauseHandlerError.h"
#include "Result.h"
#include "pkb/PKB.h"

using namespace std;

class FollowsStarHandler : public ClauseHandler {
private:
    void validate();

public:
    FollowsStarHandler(Clause *clause, PKB *pkb)
        : ClauseHandler(clause, pkb) {}
    Result eval();
};
