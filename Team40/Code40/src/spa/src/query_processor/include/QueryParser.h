#pragma once

#include <algorithm>
#include <vector>

#include "Abstractions.h"
#include "Clause.h"
#include "ClauseTypeHelper.h"
#include "DesignEntityTypeHelper.h"
#include "Reference.h"

using namespace std;

class QueryParser {
private:
    DesignEntityTypeHelper deHelper;
    ClauseTypeHelper clsHelper;

    ReferenceType checkRefType(string val);
    bool isInteger(string val);
    bool isNamedSynonym(string val);
    bool isWildcard(string val);

public:
    QueryParser() = default;
    Reference *parseDeclaration(DeclPair declaration);
    Clause *parseClause(ClsTuple clause, vector<Reference *> &declList);
};
