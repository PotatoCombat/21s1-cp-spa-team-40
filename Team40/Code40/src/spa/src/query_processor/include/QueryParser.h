#pragma once

#include <algorithm>

#include "Abstractions.h"
#include "DesignEntityTypeHelper.h"
#include "Reference.h"
#include "Relation.h"
#include "RelationTypeHelper.h"

using namespace std;

class QueryParser {
private:
    DesignEntityTypeHelper deHelper;
    RelationTypeHelper relHelper;

    ReferenceType checkRefType(string val);
    bool isInteger(string val);
    bool isNamedSynonym(string val);
    bool isWildcard(string val);

public:
    QueryParser();
    Reference parseDeclaration(DeclPair declaration);
    Relation parseRelation(RelTuple clause, vector<Reference> &declList);
    // PatternClause parsePatternClause(PatTuple clause);
};
