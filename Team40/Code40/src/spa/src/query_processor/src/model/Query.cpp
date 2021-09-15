#include "query_processor/model/Query.h"

Query::Query() {
    Reference *returnReference = NULL;
}

vector<Reference *> Query::getReferences() { return referenceList; }

void Query::setReturnReference(Reference *reference) {
    this->returnReference = reference;
    addReference(reference);
}

Reference *Query::getReturnReference() { return this->returnReference; }

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

void Query::addPattern(PatternClause *pattern) {
    patternList.push_back(pattern);
    addReference(pattern->getStmt());
    addReference(pattern->getVar());
}

vector<Pattern *> Query::getPatterns() { return patternList; }
