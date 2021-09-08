#pragma once

#include "../EntityReference.h"
#include "../Expression.h"
#include "../relation/Relation.h"
#include <string>

using namespace std;

// TODO: maybe have a handler to handle different types of patterns

class PatternClause {
private:
    string syn;
    EntityReference entRef;
    Expression expr;

public:
    PatternClause(string syn, EntityReference entRef, Expression expr);

    string getSynonym();
    EntityReference getEntRef();
    Expression getExpression();
};
