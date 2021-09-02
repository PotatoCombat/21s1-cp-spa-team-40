#pragma once

#include '../relation/Relation.h'

using namespace std;

class SuchThatClause {
private:
    Relation rel;

public:
    SuchThatClause() = 0;
    SuchThatClause(Relation rel);

    Relation getRelation() { return rel; }

    void setRelation(Relation rel) { this->rel = rel; }
};
