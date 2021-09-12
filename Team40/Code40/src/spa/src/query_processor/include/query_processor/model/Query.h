#pragma once

#include <string>
#include <vector>

#include "Clause.h"
#include "Reference.h"

using namespace std;

class Query {
private:
    vector<Reference *> referenceList;
    vector<Clause *> clauseList;
    Reference *returnReference;

    void addReference(Reference *reference);

public:
    Query();
    vector<Reference *> getReferences();
    // returnReference must have ReferenceType::SYNONYM, should only be called once for each query object
    void setReturnReference(Reference *reference);
    Reference *getReturnReference();
    void addClause(Clause *relation);
    vector<Clause *> getClauses();
};
