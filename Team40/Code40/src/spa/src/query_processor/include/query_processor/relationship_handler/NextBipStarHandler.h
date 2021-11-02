#pragma once

#include "ClauseHandler.h"
#include "pkb/PKB.h"
#include <queue>
#include <unordered_set>

class NextBipStarHandler;

typedef unordered_set<ProgLineIndex> (NextBipStarHandler::*ExplorationFunction)(
    ProgLineIndex progLineIndex, vector<ProgLineIndex> &callStack);

class NextBipStarHandler : public ClauseHandler {
private:
    set<string> getR1ClauseR2(string r2) override;
    set<string> getR2ClausedR1(string r1) override;
    bool isR1ClauseR2(string r1, string r2) override;
    void depthFirstSearch(ExplorationFunction explore, ProgLineIndex progLine,
                          set<string> &result,
                          unordered_set<ProgLineIndex> &visited,
                          vector<ProgLineIndex> &validBranchLines);
    unordered_set<ProgLineIndex>
    getNextBipLines(ProgLineIndex progLineIndex,
                    vector<ProgLineIndex> &validBranchBackLines);
    unordered_set<ProgLineIndex>
    getPreviousBipLines(ProgLineIndex progLineIndex,
                        vector<ProgLineIndex> &validBranchInLines);

    unordered_set<ProgLineIndex> getBranchInLines(ProgLineIndex branchBackFrom,
                                                  ProgLineIndex branchBackTo);

public:
    NextBipStarHandler(Clause *clause, PKB *pkb);
};
