#pragma once

#include <string>
#include <vector>

#include "model/Query.h"
#include "model/entity/Entity.h"
#include "model/relationship/Relationship.h"
#include "../PKB.h"
#include "Result.h"
#include "relationship_handler/RelationshipHandler.h"
#include "relationship_handler/FollowsHandler.h"

using namespace std;

class QueryEvaluator {
private:
    void combineResult(vector<vector<string>> &results, vector<Entity *> &entities, vector<string> result,
                       Entity* entity, vector<bool> &entitiesAppearInQuery);

public:
    vector<string> evaluateQuery(Query query);
};
