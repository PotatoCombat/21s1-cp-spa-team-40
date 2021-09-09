#pragma once

#include <string>
#include <vector>

#include "Reference.h"
#include "Relation.h"

using namespace std;

class Query {
private:
    vector<Reference *> referenceList;
    vector<Relation *> relationList;
    Reference *returnReference;

    void addReference(Reference *reference);

public:
    Query();
    vector<Reference *> getReferences();
    // returnReference must have ReferenceType::SYNONYM, should only be called once for each query object
    void setReturnReference(Reference *reference);
    Reference *getReturnReference();
    void addRelation(Relation *relation);
    vector<Relation *> getRelations();
};
