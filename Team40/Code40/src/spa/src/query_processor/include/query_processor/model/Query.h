#pragma once

#include <string>
#include <vector>

#include "Clause.h"
#include "PatternClause.h"
#include "Reference.h"

using namespace std;

class Query {
private:
    vector<Reference *> referenceList;
    vector<Clause *> clauseList;

    vector<Reference *> returnRefs;

    void addReference(Reference *reference);

public:
    Query();
    vector<Reference *> getReferences();
    void addReturnReference(Reference *reference);
    vector<Reference *> getReturnReferences();
    void addClause(Clause *clause);
    vector<Clause *> getClauses();
    void addPattern(PatternClause *pattern);
    vector<PatternClause *> getPatterns();
};
