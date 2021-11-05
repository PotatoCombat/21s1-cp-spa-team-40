#pragma once

#include <set>
#include <string>

#include "pkb/PKB.h"

#include "query_processor/relationship_handler/ClauseHandler.h"

using namespace std;

class ParentStarHandler : public ClauseHandler {
private:
    set<string> getR1ClauseR2(string r2) override;
    set<string> getR2ClausedR1(string r1) override;
    bool isR1ClauseR2(string r1, string r2) override;

public:
    ParentStarHandler(Clause *clause, PKB *pkb, ResultCache *cache);
};
