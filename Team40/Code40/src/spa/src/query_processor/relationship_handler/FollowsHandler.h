#pragma once

#include <string>

#include "../model/relationship/Relationship.h"
#include "../Result.h"
#include "../model/entity/Entity.h"
#include "../../PKB.h"
#include "RelationshipHandler.h"

using namespace std;

class FollowsHandler : public RelationshipHandler {
public:
	FollowsHandler(Relationship* relationship);
	Result eval();
};
