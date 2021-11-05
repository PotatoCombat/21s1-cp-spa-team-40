#pragma once

#include <queue>
#include <set>
#include <string>
#include <unordered_set>

#include "pkb/PKB.h"

#include "query_processor/relationship_handler/ClauseHandler.h"

using namespace std;

class AffectsStarHandler : public ClauseHandler {
private:
    set<string> getR1ClauseR2(string r2) override;
    set<string> getR2ClausedR1(string r1) override;
    bool isR1ClauseR2(string r1, string r2) override;

    set<ProgLineIndex> getAffectingStmts(ProgLineIndex line2);
    set<ProgLineIndex> getAffectedStmts(ProgLineIndex line1);
    bool affects(ProgLineIndex line1, ProgLineIndex line2);

public:
    AffectsStarHandler(Clause *clause, PKB *pkb, ResultCache *cache);
};
