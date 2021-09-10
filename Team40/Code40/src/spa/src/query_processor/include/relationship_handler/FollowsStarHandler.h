#pragma once

#include <string>
#include <vector>

#include "Reference.h"
#include "Clause.h"
#include "ClauseHandler.h"
#include "Result.h"
#include "pkb/PKB.h"

using namespace std;

class FollowsStarHandler : public ClauseHandler {
public:
    FollowsStarHandler(Clause *clause, PKB *pkb)
        : ClauseHandler(clause, pkb) {}
    Result eval();
};
