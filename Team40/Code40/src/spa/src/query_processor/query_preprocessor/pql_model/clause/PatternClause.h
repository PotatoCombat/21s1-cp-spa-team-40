#pragma once

#include '../relation/Relation.h'
#include '../EntityReference.h'

using namespace std;

class PatternClause {
private:
    string syn;
    EntityReference entRef;
    // Expression expr;

public:
    PatternClause() = 0;
    PatternClause(string syn, EntityReference entRef);

    string getSynonym() { return syn; }
    EntityReference getEntRef() { return entRef; }

    void setSynonym(string syn) { this->syn = syn; }
    void setEntRef(EntityReference entRef) { this->entRef = entRef; }
};
