#pragma once

#include "../relation/Relation.h"
#include "Clause.h"

using namespace std;

class SuchThatClause : public Clause {
private:
    Relation rel;

public:
    SuchThatClause(Relation rel);

    Relation getRelation();
};
