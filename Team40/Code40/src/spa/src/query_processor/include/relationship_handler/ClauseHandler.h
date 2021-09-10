#pragma once

#include "Clause.h"
#include "Result.h"
#include "pkb/PKB.h"

class ClauseHandler {
protected:
	Clause* clause;
    PKB* pkb;
	ClauseHandler(Clause* clause, PKB* pkb) : clause(clause), pkb(pkb) { }

public:
	// evaluates the clause and writes the answer to the result object
	// Result invalid = false only happens when neither of the references is synonym
	virtual Result eval() = 0;
};
