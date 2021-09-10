#pragma once

#include <set>
#include <string>
#include <vector>

#include "Reference.h"
#include "Relation.h"
#include "RelationshipHandler.h"
#include "RelationHandlerError.h"
#include "Result.h"
#include "pkb/PKB.h"

using namespace std;

class ParentHandler : public RelationshipHandler {
private:
    void validate();

public:
    ParentHandler(Relation *relationship, PKB *pkb)
        : RelationshipHandler(relationship, pkb) {}
    Result eval();
};
