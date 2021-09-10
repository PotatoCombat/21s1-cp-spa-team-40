#pragma once

#include <string>
#include <vector>

#include "Relation.h"
#include "Result.h"
#include "Reference.h"
#include "RelationshipHandler.h"
#include "RelationHandlerError.h"
#include "pkb/PKB.h"

using namespace std;

class FollowsHandler : public RelationshipHandler {
public:
    FollowsHandler(Relation *relationship, PKB* pkb) : RelationshipHandler(relationship, pkb) {}
	Result eval();
};
