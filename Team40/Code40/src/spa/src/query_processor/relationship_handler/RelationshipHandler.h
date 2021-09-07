#pragma once

#include "../model/relationship/Relationship.h"

class RelationshipHandler {
protected:
	Relationship* relationship;
public:
	virtual Result eval() = 0;
};
