#include "query_processor/model/Query.h"

vector<Reference *> Query::getReferences() { return referenceList; }

void Query::addReturnReference(Reference *reference) {
    this->returnRefs.push_back(reference);
    addReference(reference);
}

vector<Reference *> Query::getReturnReferences() { return this->returnRefs; }

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

bool Query::equals(Query &other) {
    if (returnRefs.size() != other.returnRefs.size() ||
        clauseList.size() != other.clauseList.size() ||
        referenceList.size() != other.referenceList.size()) {
        return false;
    }

    for (int i = 0; i < returnRefs.size(); ++i) {
        Reference a = *returnRefs.at(i);
        Reference b = *other.returnRefs.at(i);
        if (!a.equals(b)) {
            return false;
        }
    }

    for (int i = 0; i < clauseList.size(); ++i) {
        Clause a = *clauseList.at(i);
        Clause b = *other.clauseList.at(i);
        if (!a.equals(b)) {
            return false;
        }
    }

    for (int i = 0; i < referenceList.size(); ++i) {
        Reference a = *referenceList.at(i);
        Reference b = *other.referenceList.at(i);
        if (!a.equals(b)) {
            return false;
        }
    }

    return true;
}

Query::~Query() {}
