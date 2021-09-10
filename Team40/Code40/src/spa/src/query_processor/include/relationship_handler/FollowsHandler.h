#pragma once

#include <string>
#include <vector>

#include "Clause.h"
#include "Result.h"
#include "Reference.h"
#include "ClauseHandler.h"
#include "pkb/PKB.h"

using namespace std;

class FollowsHandler : public ClauseHandler {
public:
    FollowsHandler(Clause *clause, PKB* pkb) : ClauseHandler(clause, pkb) {}
	Result eval();
};
