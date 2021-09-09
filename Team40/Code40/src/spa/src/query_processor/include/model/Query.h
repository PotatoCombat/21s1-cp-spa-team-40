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
    void setReturnReference(Reference *reference); // returnReference must have ReferenceType::SYNONYM
    Reference *getReturnReference();
    void addRelation(Relation *relation);
    vector<Relation *> getRelations();
};
