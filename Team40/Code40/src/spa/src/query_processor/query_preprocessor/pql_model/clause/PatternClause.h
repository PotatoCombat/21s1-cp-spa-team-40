#pragma once

#include "../EntityReference.h"
#include "../Expression.h"
#include "../relation/Relation.h"
#include "Clause.h"
#include <string>

using namespace std;

class PatternClause : public Clause {
private:
    string syn;
    EntityReference entRef;
    Expression expr;

public:
    PatternClause(string syn, EntityReference entRef, Expression expr);

    string getSynonym();
    EntityReference getEntRef();
    Expression getExpression();

    void setSynonym(string syn);
    void setEntRef(EntityReference entRef);
    void setExpression(Expression expr);
};
