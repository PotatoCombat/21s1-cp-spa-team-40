#pragma once

#include "Relationship.h"
#include "PKB.h"

class RelationshipHandler {
protected:
	Relationship* relationship;
    PKB* pkb;
	RelationshipHandler(Relationship* relationship, PKB* pkb) {
        this->relationship = relationship;
        this->pkb = pkb;
	}

public:
	// evaluates the relationship and writes the answer to the result object
	Result eval() = 0;
};
