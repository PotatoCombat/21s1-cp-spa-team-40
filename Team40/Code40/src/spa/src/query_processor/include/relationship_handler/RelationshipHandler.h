#pragma once

#include "../model/relationship/Relationship.h"
#include "../../pkb/include/PKB.h"

class RelationshipHandler {
protected:
	Relationship* relationship;
    PKB pkb;
	RelationshipHandler(Relationship* relationship, PKB pkb) {
        this->relationship = relationship;
        this->pkb = pkb;
	}

public:
	virtual Result eval() = 0;
};
