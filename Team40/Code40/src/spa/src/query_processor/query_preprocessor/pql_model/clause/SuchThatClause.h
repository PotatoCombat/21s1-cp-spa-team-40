#pragma once

#include "../relation/Relation.h"

using namespace std;

class SuchThatClause {
private:
    Relation rel;

public:
    SuchThatClause();
    SuchThatClause(Relation rel);

    Relation getRelation();

    void setRelation(Relation rel);
};
