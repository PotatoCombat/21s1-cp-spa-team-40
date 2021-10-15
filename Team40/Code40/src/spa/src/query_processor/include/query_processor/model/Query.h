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
    vector<Reference *> returnRefs;

    void addReference(Reference *reference);

public:
    Query() = default;
    vector<Reference *> getReferences();
    void addReturnReference(Reference *reference);
    vector<Reference *> getReturnReferences();
    void addClause(Clause *clause);
    vector<Clause *> getClauses();
    bool equals(Query &other);
    ~Query();
};
