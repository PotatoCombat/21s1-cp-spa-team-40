#pragma once

#include "Abstractions.h"
#include "DesignEntityTypeHelper.h"
#include "Reference.h"
#include "Relation.h"
#include "RelationTypeHelper.h"

using namespace std;

class QueryParser {
private:
    DesignEntityTypeHelper deTypeHelper;
    RelationTypeHelper relTypeHelper;

public:
    QueryParser();
    Reference parseDeclaration(DeclTuple declaration);
    Relation parseSuchThatClause(RelTuple clause, vector<Reference> &declList);
    // PatternClause parsePatternClause(PatTuple clause);
};
