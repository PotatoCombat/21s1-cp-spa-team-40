#pragma once

#include "Relation.h"
#include "pkb/PKB.h"

class RelationshipHandler {
protected:
	Relation* relation;
    PKB* pkb;
	RelationshipHandler(Relation* relation, PKB* pkb) {
        this->relation = relation;
        this->pkb = pkb;
	}

public:
	// evaluates the relationship and writes the answer to the result object
	virtual Result eval() = 0;
};
