#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

#include "model/Query.h"
#include "model/Reference.h"
#include "model/Relation.h"
#include "pkb/PKB.h"
#include "Result.h"
#include "relationship_handler/RelationshipHandler.h"
#include "relationship_handler/FollowsHandler.h"
#include "Util.h"

using namespace std;

class QueryEvaluator {
private:
    PKB pkb;

    void combineResult(vector<vector<string>> &results, vector<Reference *> &entities, vector<string> result,
                       Reference* entity, vector<bool> &entitiesAppearInQuery);  

public:
    QueryEvaluator(PKB pkb);
    vector<string> evaluateQuery(Query query);
};
