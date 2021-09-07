#pragma once

#include <string>

#include "Relationship.h"
#include "Result.h"
#include "Entity.h"
#include "RelationshipHandler.h"
#include "PKB.h"
#include "Util.h"

using namespace std;

class FollowsHandler : public RelationshipHandler {
public:
    FollowsHandler(Relationship *relationship, PKB* pkb) : RelationshipHandler(relationship, pkb) {}
	Result eval();
};
