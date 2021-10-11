#include "query_processor/model/Query.h"

Query::Query() {
    Reference *returnReference = NULL;
}

vector<Reference *> Query::getReferences() { return referenceList; }

void Query::addReturnReference(Reference *reference) {
    this->returnRefs.push_back(reference);
    addReference(reference);
}

vector<Reference*> Query::getReturnReferences() { return this->returnRefs; }

void Query::addClause(Clause *clause) {
    clauseList.push_back(clause);
    addReference(clause->getFirstReference());
    addReference(clause->getSecondReference());
}

void Query::addReference(Reference *reference) {
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
