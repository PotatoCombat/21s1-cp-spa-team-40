#pragma once

#include <queue>
#include <set>
#include <string>
#include <unordered_set>

#include "ClauseHandler.h"

#include "pkb/PKB.h"

using namespace std;

class AffectsHandler : public ClauseHandler {
private:
    set<string> getR1ClauseR2(string r2) override;
    set<string> getR2ClausedR1(string r1) override;
    bool isR1ClauseR2(string r1, string r2) override;

    set<string> breathFirstSearch(const ProgLineIndex line, const set<VarName> &vars, bool isFindingR1);
    void exploreNeighbours(bool isFindingR1, const ProgLineIndex currLine,
                           const VarName currVar,
                           unordered_set<ProgLineIndex> &visited,
                           queue<ProgLineIndex> &open, set<string> &results);

public:
    AffectsHandler(Clause *clause, PKB *pkb);
};
