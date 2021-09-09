#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

#include "common/model/statement/Statement.h"
#include "pkb/PKB.h"

#include "model/Query.h"
#include "model/Reference.h"
#include "model/Relation.h"
#include "Result.h"
#include "relationship_handler/RelationshipHandler.h"
#include "relationship_handler/FollowsHandler.h"
#include "relationship_handler/FollowsStarHandler.h"
#include "relationship_handler/ModifiesStmtHandler.h"
#include "relationship_handler/ParentHandler.h"
#include "relationship_handler/ParentStarHandler.h"
#include "relationship_handler/UsesStmtHandler.h"

using namespace std;

class QueryEvaluator {
private:
    PKB* pkb;

    void combineResult(vector<vector<string>> &results, vector<Reference *> &references, vector<string> result,
                       Reference* reference, vector<bool> &referenceAppearInClauses);  

    void toString(vector<int> &vectorIn, vector<string> &vectorOut);

public:
    QueryEvaluator(PKB* pkb);
    vector<string> evaluateQuery(Query query);
};
