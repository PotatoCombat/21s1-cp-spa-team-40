#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

#include "model/Query.h"
#include "model/entity/Entity.h"
#include "model/relationship/Relationship.h"
#include "PKB.h"
#include "Result.h"
#include "relationship_handler/RelationshipHandler.h"
#include "relationship_handler/FollowsHandler.h"

using namespace std;

void toString(vector<int> vectorIn, vector<string> vectorOut);

class QueryEvaluator {
private:
    PKB pkb;

    void combineResult(vector<vector<string>> &results, vector<Entity *> &entities, vector<string> result,
                       Entity* entity, vector<bool> &entitiesAppearInQuery);  

public:
    QueryEvaluator(PKB pkb);
    vector<string> evaluateQuery(Query query);
};
