#pragma once

#include <set>
#include <string>

#include "common/model/Abstractions.h"

#include "pkb/PKB.h"

#include "query_processor/relationship_handler/ClauseHandler.h"

using namespace std;

class FollowsHandler : public ClauseHandler {
private:
    set<string> getR1ClauseR2(string r2) override;
    set<string> getR2ClausedR1(string r1) override;
    bool isR1ClauseR2(string r1, string r2) override;

public:
    FollowsHandler(Clause *clause, PKB *pkb);
};
