#pragma once

#include <string>
#include <vector>

#include "reference/Reference.h"
#include "relation/Relation.h"

using namespace std;

class Query {
private:
    vector<Relation *> relationList;
    Reference *returnReference;

public:
    Query();

    vector<Reference *> getReferences();

    void setReturnReference(Reference *reference);
    Reference *getReturnReference();

    void addRelation(Relation *relation);
    vector<Relation *> getRelations();
};
