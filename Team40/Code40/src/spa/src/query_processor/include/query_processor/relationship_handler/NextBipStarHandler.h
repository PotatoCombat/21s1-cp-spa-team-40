#pragma once

#include "ClauseHandler.h"
#include "pkb/PKB.h"
#include <queue>
#include <unordered_set>

class NextBipStarHandler : public ClauseHandler {
private:
    set<string> getR1ClauseR2(string r2) override;
    set<string> getR2ClausedR1(string r1) override;
    bool isR1ClauseR2(string r1, string r2) override;

public:
    NextBipStarHandler(Clause *clause, PKB *pkb);
};
