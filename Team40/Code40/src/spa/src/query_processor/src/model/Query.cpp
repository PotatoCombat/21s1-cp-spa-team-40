#include "Query.h"

Query::Query() {
    vector<Relation *> relationList;
    Reference *returnReference;
}

vector<Reference *> Query::getReferences() {
    return vector<Reference *>(); // TODO: remove later
}

void Query::setReturnReference(Reference *reference) {
    this->returnReference = reference;
}

Reference *Query::getReturnReference() { return this->returnReference; }

void Query::addRelation(Relation *relation) {
    relationList.push_back(relation);
}

vector<Relation *> Query::getRelations() { return this->relationList; }
