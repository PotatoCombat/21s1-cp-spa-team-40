#pragma once

#include "Relation.h"
#include "RelationHandlerError.h"
#include "Result.h"
#include "pkb/PKB.h"
#include "common/model/statement/Statement.h"

class RelationshipHandler {
protected:
	Relation* relation;
    PKB* pkb;
	RelationshipHandler(Relation* relation, PKB* pkb) : relation(relation), pkb(pkb) { }

public:
	// evaluates the relationship and writes the answer to the result object
	// Result invalid = false only happens when neither of the references is synonym
	virtual Result eval() = 0;
    static StatementType desTypeToStmtType(DesignEntityType desType);
	static bool isDesTypeStmtType(DesignEntityType desType, StatementType stmtType);
};
