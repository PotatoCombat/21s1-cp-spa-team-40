#pragma once

#include "../relation/Relation.h"
#include "../EntityReference.h"
#include <string>

using namespace std;

class PatternClause {
private:
    string syn;
    EntityReference entRef;
    // Expression expr;

public:
    PatternClause();
    PatternClause(string syn, EntityReference entRef);

    string getSynonym();
    EntityReference getEntRef();

    void setSynonym(string syn);
    void setEntRef(EntityReference entRef);
};
