#pragma once

#include <string>
#include <vector>

#include "Reference.h"
#include "Relation.h"
#include "RelationshipHandler.h"
#include "Result.h"
#include "pkb/PKB.h"

using namespace std;

class ModifiesProcHandler : public RelationshipHandler {
public:
    ModifiesProcHandler(Relation *relationship, PKB *pkb)
        : RelationshipHandler(relationship, pkb) {}
    Result eval();
};
