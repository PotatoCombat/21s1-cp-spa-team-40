#include "Query.h"

Query::Query() {
    vector<Relation *> relationList;
    Reference *returnReference = NULL;
    vector<Reference *> referenceList;
}

vector<Reference *> Query::getReferences() { return referenceList; }

void Query::setReturnReference(Reference *reference) {
    this->returnReference = reference;
    addReference(reference);
}

Reference *Query::getReturnReference() { return this->returnReference; }

void Query::addRelation(Relation *relation) {
    relationList.push_back(relation);
    addReference(relation->getFirstReference());
    addReference(relation->getSecondReference());
}

void Query::addReference(Reference* reference) {
    if (reference->getRefType() != ReferenceType::SYNONYM) {
        return;
    }

    for (auto existingReference : referenceList) {
        if (existingReference->equals(*reference)) {
            return;
        }
    }
    referenceList.push_back(reference);
}

vector<Relation *> Query::getRelations() { return this->relationList; }
