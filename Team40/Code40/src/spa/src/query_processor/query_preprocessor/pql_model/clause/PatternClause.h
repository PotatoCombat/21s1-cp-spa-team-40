#pragma once

#include "../EntityReference.h"
#include "../Expression.h"
#include "../relation/Relation.h"
#include <string>

using namespace std;

class PatternClause {
private:
    string syn;
    EntityReference entRef;
    Expression expr;

public:
    PatternClause();
    PatternClause(string syn, EntityReference entRef, Expression expr);

    string getSynonym();
    EntityReference getEntRef();
    Expression getExpression();

    void setSynonym(string syn);
    void setEntRef(EntityReference entRef);
    void setExpression(Expression expr);
};
