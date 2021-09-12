#include "query_processor/model/Query.h"

Query::Query() {
    vector<Clause *> clauseList;
    Reference *returnReference = NULL;
    vector<Reference *> referenceList;
}

vector<Reference *> Query::getReferences() { return referenceList; }

void Query::setReturnReference(Reference *reference) {
    this->returnReference = reference;
    addReference(reference);
}

Reference *Query::getReturnReference() { return this->returnReference; }

void Query::addClause(Clause *relation) {
    clauseList.push_back(relation);
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

vector<Clause *> Query::getClauses() { return this->clauseList; }
