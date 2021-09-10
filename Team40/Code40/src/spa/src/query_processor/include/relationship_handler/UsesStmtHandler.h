#pragma once

#include <string>
#include <vector>

#include "Reference.h"
#include "Relation.h"
#include "RelationshipHandler.h"
#include "RelationHandlerError.h"
#include "Result.h"
#include "pkb/PKB.h"

using namespace std;

class UsesStmtHandler : public RelationshipHandler {
public:
    UsesStmtHandler(Relation *relationship, PKB *pkb)
        : RelationshipHandler(relationship, pkb) {}
    Result eval();
};
