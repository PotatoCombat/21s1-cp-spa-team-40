#pragma once

#include "ClauseHandler.h"
#include "pkb/PKB.h"
#include <queue>
#include <unordered_set>

class NextBipStarHandler;

typedef set<ProgLineIndex> (NextBipStarHandler::*ExplorationFunction)(
    ProgLineIndex progLineIndex);

class NextBipStarHandler : public ClauseHandler {
private:
    set<string> getR1ClauseR2(string r2) override;
    set<string> getR2ClausedR1(string r1) override;
    bool isR1ClauseR2(string r1, string r2) override;
    set<string> breadthFirstSearch(ExplorationFunction explorationFunction,
                                   const string &r);
    set<ProgLineIndex> getNextBipLines(ProgLineIndex progLineIndex);
    set<ProgLineIndex> getPreviousBipLines(ProgLineIndex progLineIndex);

public:
    NextBipStarHandler(Clause *clause, PKB *pkb);
};
