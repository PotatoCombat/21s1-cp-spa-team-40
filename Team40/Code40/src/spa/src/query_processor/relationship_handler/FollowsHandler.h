#pragma once

#include <string>

#include "../model/relationship/Relationship.h"
#include "../Result.h"
#include "../model/entity/Entity.h"
#include "RelationshipHandler.h"
#include "PKB.h"

using namespace std;

class FollowsHandler : public RelationshipHandler {
public:
    FollowsHandler(Relationship *relationship, PKB pkb) : RelationshipHandler(relationship, pkb) {}
	Result eval();
};
