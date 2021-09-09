#pragma once

#include "Relation.h"
#include "pkb/PKB.h"

class RelationshipHandler {
protected:
	Relation* relation;
    PKB* pkb;
	RelationshipHandler(Relation* relation, PKB* pkb) : relation(relation), pkb(pkb) { }

public:
	// evaluates the relationship and writes the answer to the result object
	// Result invalid = false only happens when neither of the references is synonym
	virtual Result eval() = 0;
};
