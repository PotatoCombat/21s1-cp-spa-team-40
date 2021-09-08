#pragma once

#include <string>

#include "Relation.h"
#include "Result.h"
#include "Reference.h"
#include "RelationshipHandler.h"
#include "pkb/PKB.h"

using namespace std;

class FollowsHandler : public RelationshipHandler {
public:
    FollowsHandler(Relation *relationship, PKB* pkb) : RelationshipHandler(relationship, pkb) {}
	Result eval();
};
