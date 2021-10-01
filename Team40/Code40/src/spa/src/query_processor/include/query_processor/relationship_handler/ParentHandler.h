#pragma once

#include <set>
#include <string>

#include "ClauseHandler.h"

#include "pkb/PKB.h"

using namespace std;

class ParentHandler : public ClauseHandler {
private:
    set<string> getR1ClauseR2(string r2) override;
    set<string> getR2ClausedR1(string r1) override;
    bool isR1ClauseR2(string r1, string r2) override;

public:
    ParentHandler(Clause *clause, PKB *pkb);
};
