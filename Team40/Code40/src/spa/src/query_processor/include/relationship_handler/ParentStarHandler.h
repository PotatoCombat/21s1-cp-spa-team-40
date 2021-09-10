#pragma once

#include <set>
#include <string>
#include <vector>

#include "Reference.h"
#include "Clause.h"
#include "ClauseHandler.h"
#include "Result.h"
#include "pkb/PKB.h"

using namespace std;

class ParentStarHandler : public ClauseHandler {
public:
    ParentStarHandler(Clause *clause, PKB *pkb)
        : ClauseHandler(clause, pkb) {}
    Result eval();
};
