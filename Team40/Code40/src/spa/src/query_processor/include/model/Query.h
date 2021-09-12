#pragma once

#include <string>
#include <vector>

#include "Reference.h"
#include "Clause.h"
#include "Pattern.h"

using namespace std;

class Query {
private:
    vector<Reference *> referenceList;
    vector<Clause *> clauseList;
    vector<Pattern *> patternList;
    
    Reference *returnReference;

    void addReference(Reference *reference);

public:
    Query();
    vector<Reference *> getReferences();
    // returnReference must have ReferenceType::SYNONYM, should only be called once for each query object
    void setReturnReference(Reference *reference);
    Reference *getReturnReference();
    void addClause(Clause *clause);
    vector<Clause *> getClauses();
    void addPattern(Pattern *pattern);
    vector<Pattern *> getPatterns();
};
