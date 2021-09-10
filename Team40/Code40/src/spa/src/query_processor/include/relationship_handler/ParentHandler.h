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

class ParentHandler : public ClauseHandler {
public:
    ParentHandler(Clause *clause, PKB *pkb)
        : ClauseHandler(clause, pkb) {}
    Result eval();
};
